/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
*/
#include <SFML/Network/Packet.hpp>
#include <SFML/System/Sleep.hpp>

#include <iostream>

#include "game_server.hpp"
#include "utility.hpp"
#include "server_protocol.hpp"
#include "client_protocol.hpp"

GameServer::GameServer()
    : m_thread(&GameServer::ExecutionThread, this)
    , m_listening_state(false)
    , m_client_timeout(sf::seconds(1.f))
    , m_max_connected_players(20)
    , m_connected_players(0)
    , m_peers(1)
    , m_waiting_thread_end(false)
{
    m_listener_socket.setBlocking(false);
    m_peers[0].reset(new RemotePeer);
}

GameServer::~GameServer()
{
    m_waiting_thread_end = true;
    m_thread.join();
}

void GameServer::SetListening(bool enable)
{
    //Check if the server is already listening
    if (enable)
    {
        if (!m_listening_state)
        {
            m_listening_state = (m_listener_socket.listen(kServerPort) == sf::TcpListener::Status::Done);
        }
    }
    else
    {
        m_listener_socket.close();
        m_listening_state = false;
    }
}

void GameServer::ExecutionThread()
{
    //Initialisation
    SetListening(true);

    sf::Time tick_rate = sf::seconds(1.f / 20.f);
    sf::Time tick_time = sf::Time::Zero;
    sf::Clock frame_clock, tick_clock;

    while (!m_waiting_thread_end)
    {
        //This is the game loop
        HandleIncomingConnections();
        HandleIncomingPackets();

        tick_time += tick_clock.getElapsedTime();
        tick_clock.restart();

        while (tick_time >= tick_rate)
        {
            Tick();
            tick_time -= tick_rate;
        }
    }
}

void GameServer::Tick()
{
	sf::Packet packet = ServerProtocol::Empty().asPacket();
}

sf::Time GameServer::Now() const
{
    return m_clock.getElapsedTime();
}

void GameServer::HandleIncomingPackets()
{
    bool detected_timeout = false;

    for (PeerPtr& peer : m_peers)
    {
        if (peer->m_ready)
        {
            sf::Packet packet;
            while (peer->m_socket.receive(packet) == sf::Socket::Status::Done)
            {
                //Interpret the packet and react to it
                ResolvePacket(packet, *peer, detected_timeout);

                peer->m_last_packet_time = Now();
                packet.clear();
            }

            if (Now() > peer->m_last_packet_time + m_client_timeout)
            {
                peer->m_timed_out = true;
                detected_timeout = true;
            }
        }
    }

    if (detected_timeout)
    {
        HandleDisconnections();
    }
}

void GameServer::ResolvePacket(sf::Packet& packet, RemotePeer& receiving_peer, bool& detected_timeout)
{
    uint8_t packet_type;
    packet >> packet_type;

    switch (static_cast<ClientProtocol::PacketType>(packet_type))
    {
    case ClientProtocol::PacketType::kEmpty:
    {
        ClientProtocol::Empty empty_packet(packet);
        break;
    }
    case ClientProtocol::PacketType::kIntroduceSelf:
    {
        ClientProtocol::IntroduceSelf intro(packet);
		receiving_peer.m_name = intro.name;
		receiving_peer.m_character_id = intro.characterId;

        std::cout << "Player " << receiving_peer.m_assigned_id << " introduced themselves as " << intro.name
			<< " with character id " << static_cast<int>(intro.characterId) << std::endl;

		sf::Packet response = ServerProtocol::PlayerJoined(receiving_peer.m_assigned_id, intro.characterId, intro.name)
            .asPacket();
		SendToAll(response);
        break;
    }
    case ClientProtocol::PacketType::kLeaveGame:
    {
        ClientProtocol::LeaveGame leave_game(packet);
        receiving_peer.m_timed_out = true;
        detected_timeout = true;
        break;
    }
    case ClientProtocol::PacketType::kLobbyReady:
    {
        ClientProtocol::LobbyReady lobby_ready(packet);
		receiving_peer.m_lobby_ready = lobby_ready.isReady;
        break;
    }
    default:
    {
        break;
    }
    }
}

void GameServer::HandleIncomingConnections()
{
    if (!m_listening_state)
    {
        return;
    }

    if (m_listener_socket.accept(m_peers[m_connected_players]->m_socket) == sf::TcpListener::Status::Done)
    {
		uint8_t assigned_id = GetFreeID();
		uint8_t assigned_team = GetFreeTeam();
		m_peers[m_connected_players]->m_assigned_id = assigned_id;
        m_peers[m_connected_players]->m_assigned_team = assigned_team;

        sf::Packet packet = ServerProtocol::WelcomePlayer(assigned_id, assigned_team).asPacket();
        m_peers[m_connected_players]->m_socket.send(packet);
        m_peers[m_connected_players]->m_ready = true;
        m_peers[m_connected_players]->m_last_packet_time = Now();

        m_connected_players++;

        if (m_connected_players >= m_max_connected_players)
        {
            SetListening(false);
        }
        else
        {
            m_peers.emplace_back(PeerPtr(new RemotePeer()));
        }
    }
}

void GameServer::HandleDisconnections()
{
    for (auto itr = m_peers.begin(); itr != m_peers.end();)
    {
        if ((*itr)->m_timed_out)
        {
            //Inform everyone of a disconnection, erase
            sf::Packet packet = ServerProtocol::PlayerLeft((*itr)->m_assigned_id).asPacket();
            SendToAll(packet);

            m_connected_players--;

            itr = m_peers.erase(itr);

            //If the number of peers has dropped below max_connections
            if (m_connected_players < m_max_connected_players)
            {
                m_peers.emplace_back(PeerPtr(new RemotePeer()));
                SetListening(true);
            }
        }
        else
        {
            ++itr;
        }
    }
}

void GameServer::SendToAll(sf::Packet& packet)
{
    for (std::size_t i = 0; i < m_connected_players; ++i)
    {
        if (m_peers[i]->m_ready)
        {
            auto _ = m_peers[i]->m_socket.send(packet);
        }
    }
}

uint8_t GameServer::GetFreeID() const
{
    for (int i = 0; i < 255; ++i)
    {
        bool id_taken = false;
        for (const PeerPtr& peer : m_peers)
        {
            if (peer->m_assigned_id == i)
            {
                id_taken = true;
                break;
            }
        }
        if (!id_taken)
        {
            return i;
        }
    }
    return 255;
}

uint8_t GameServer::GetFreeTeam() const
{
    int teamOne = 0;
    int teamTwo = 0;
    for (const PeerPtr& peer : m_peers)
    {
        if (peer->m_assigned_team == 0)
        {
            teamOne++;
        }
        else
        {
            teamTwo++;
        }
    }
    return (teamOne <= teamTwo) ? 0 : 1;
}

//It is essential to set the sockets to non-blocking - m_socket.setBlocking(false)
//otherwise the server will hang waiting to read input from a connection
GameServer::RemotePeer::RemotePeer()
    : m_ready(false)
    , m_timed_out(false)
    , m_assigned_id(255)
	, m_assigned_team(0)
    , m_character_id(0)
    , m_lobby_ready(false)
    , m_name("")
{
    m_socket.setBlocking(false);
}
