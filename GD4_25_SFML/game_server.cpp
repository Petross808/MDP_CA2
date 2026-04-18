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
#include "constants.hpp"

GameServer::GameServer()
    : m_thread(nullptr)
    , m_listening_state(false)
    , m_client_timeout(sf::seconds(3.f))
    , m_max_connected_players(20)
    , m_connected_players(0)
    , m_peers(1)
    , m_waiting_thread_end(false)
    , m_server_running(false)
	, m_in_game(false)
    , m_world_sim(nullptr)
    , m_game_data()
{
    m_listener_socket.setBlocking(false);
    m_game_data.SetNetworkMode(NetworkMode::kServer);
}

GameServer::~GameServer()
{
    End();
}

void GameServer::Start()
{
    std::cout << "Server Start" << std::endl;
    if (m_server_running) return;
    m_peers.clear();
    m_peers.emplace_back(new RemotePeer);
    m_server_running = true;
    m_waiting_thread_end = false;
	m_game_data.SetLevel(0);
    m_thread = std::make_unique<std::thread>(& GameServer::ExecutionThread, this);
}

void GameServer::End()
{
    if (m_thread)
    {
        m_waiting_thread_end = true;
        m_thread->join();
        m_thread.release();
        m_server_running = false;
    }
    SetListening(false);
    m_peers.clear();
    m_connected_players = 0;
    std::cout << "Server End" << std::endl;
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

    sf::Time frame_rate = sf::seconds(kTimePerFrame);
    sf::Time frame_time = sf::Time::Zero;
    sf::Time tick_rate = sf::seconds(1.f / 15.f);
    sf::Time tick_time = sf::Time::Zero;
    sf::Clock frame_clock, tick_clock;

    while (!m_waiting_thread_end)
    {
        //This is the game loop
        HandleIncomingConnections();
        HandleIncomingPackets();

        frame_time += frame_clock.getElapsedTime();
        frame_clock.restart();
        tick_time += tick_clock.getElapsedTime();
        tick_clock.restart();

        //Fixed time step
        while (frame_time >= frame_rate)
        {
            Frame(frame_rate);
            frame_time -= frame_rate;
        }

        while (tick_time >= tick_rate)
        {
            Tick();
            tick_time -= tick_rate;
        }
    }
}

void GameServer::Frame(sf::Time dt)
{
    if (m_world_sim)
    {
        m_world_sim->Update(dt);
        auto& collisions = m_world_sim->GetCollisionData();

        if (collisions.size() > 0)
        {
            sf::Packet collisionSync = ServerProtocol::CollisionSync(collisions).asPacket();
            SendToAll(collisionSync);
        }

        for (auto& peer : m_peers)
        {
            if (peer->m_ready)
            {
                peer->m_player_controller.Update(m_world_sim->GetCommandQueue());
            }
        }
        
        if (m_world_sim->CheckScore())
        {
            ResetLobby();
        }
    }

    m_network_tracker.Update(dt);
}

void GameServer::Tick()
{
    if (!m_in_game)
    {
        ReadyCheck();
    }
    else
    {
        if (m_world_sim)
        {
            sf::Packet packet = ServerProtocol::PhysicsSync(m_world_sim->GetPhysicsState()).asPacket();
            SendToAll(packet);
        }
    }
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
                m_network_tracker.LogReceivedPacket(packet);

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
		receiving_peer.m_player_data.name = intro.name;
		receiving_peer.m_player_data.character = intro.characterId;

        std::cout << "Player " << receiving_peer.m_player_data.id << " introduced themselves as " << intro.name
			<< " with character id " << static_cast<int>(intro.characterId) << std::endl;

		sf::Packet response = ServerProtocol::PlayerJoined(
            receiving_peer.m_player_data.id,
            receiving_peer.m_player_data.character,
            receiving_peer.m_player_data.name,
            receiving_peer.m_player_data.team
        ).asPacket();
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
    case ClientProtocol::PacketType::kLobbyUpdateSelf:
    {
        ClientProtocol::LobbyUpdateSelf lobby_update(packet);
		receiving_peer.m_player_data.lobby_ready = lobby_update.isReady;
		receiving_peer.m_player_data.character = lobby_update.characterId;
		receiving_peer.m_player_data.name = lobby_update.name;
        
        sf::Packet response = ServerProtocol::LobbyPlayerUpdate(
            receiving_peer.m_player_data.id,
            receiving_peer.m_player_data.lobby_ready,
            receiving_peer.m_player_data.character,
			receiving_peer.m_player_data.name
        ).asPacket();
        SendToAll(response);
        break;
    }
    case ClientProtocol::PacketType::kChangeLevel:
    {
        ClientProtocol::ChangeLevel change_level(packet);
        m_game_data.SetLevel(change_level.levelId);
        break;
    }
    case ClientProtocol::PacketType::kActionSelf:
    {
        ClientProtocol::ActionSelf action_self(packet);
        receiving_peer.m_player_controller.ApplyNetworkInput(action_self.actionId, action_self.isPressed, action_self.isRealTime);

        sf::Packet response = ServerProtocol::ActionPlayer(
            receiving_peer.m_player_data.id,
            action_self.actionId,
            action_self.isPressed,
            action_self.isRealTime).asPacket();

        SendToAll(response);
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
		m_peers[m_connected_players]->m_player_data.id = assigned_id;
        m_peers[m_connected_players]->m_player_data.team = assigned_team;
        m_peers[m_connected_players]->m_player_controller.SetID(assigned_id);
		std::vector<PlayerData> player_list;
        for(auto& peer : m_peers)
        {
            if (peer->m_ready)
            {
                player_list.push_back(peer->m_player_data);
            }
        }

        sf::Packet packet = ServerProtocol::WelcomePlayer(assigned_id, assigned_team, player_list).asPacket();
        m_network_tracker.LogSentPacket(packet);
        auto _ = m_peers[m_connected_players]->m_socket.send(packet);
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
            sf::Packet packet = ServerProtocol::PlayerLeft((*itr)->m_player_data.id).asPacket();
            SendToAll(packet);
            m_connected_players--;
            itr = m_peers.erase(itr);
        }
        else
        {
            ++itr;
        }
    }

    //If the number of peers has dropped below max_connections
    if (m_connected_players == m_max_connected_players - 1)
    {
        m_peers.emplace_back(PeerPtr(new RemotePeer()));
        SetListening(true);
    }
}

void GameServer::SendToAll(sf::Packet& packet)
{
    for (std::size_t i = 0; i < m_connected_players; ++i)
    {
        if (m_peers[i]->m_ready)
        {
            m_network_tracker.LogSentPacket(packet);
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
            if (peer->m_player_data.id == i)
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
        if (peer->m_ready)
        {
            if (peer->m_player_data.team == 0)
            {
                teamOne++;
            }
            else
            {
                teamTwo++;
            }
        }
    }
    return (teamOne <= teamTwo) ? 0 : 1;
}

void GameServer::ReadyCheck()
{
    bool all_ready = true;
    for (const PeerPtr& peer : m_peers)
    {
        if (peer->m_ready && !peer->m_player_data.lobby_ready)
        {
            all_ready = false;
            break;
        }
	}

    if (all_ready && m_connected_players > 1)
    {
        uint64_t seed = Now().asMicroseconds();
        m_game_data.Reset();
        m_game_data.SetSeed(seed);
        m_world_sim.reset(new WorldSimulation(m_game_data));

        int team_one_players = 0;
        int team_two_players = 0;
        for (auto& peer : m_peers)
        {
            if (peer->m_ready)
            {
                if (peer->m_player_data.team == 0)
                {
                    ++team_one_players;
                }
                else
                {
                    ++team_two_players;
                }
            }
        }

        for (auto& peer : m_peers)
        {
            if (peer->m_ready)
            {
                if (peer->m_player_data.team == 0)
                {
                    m_world_sim->SpawnPlayerPawn(peer->m_player_data.team, peer->m_player_data.id, peer->m_player_data.character, team_one_players);
                }
                else
                {
                    m_world_sim->SpawnPlayerPawn(peer->m_player_data.team, peer->m_player_data.id, peer->m_player_data.character, team_two_players);
                }
            }
        }

        m_in_game = true;
        std::cout << "All players ready, starting game!" << std::endl;
		sf::Packet packet = ServerProtocol::GameStart(m_game_data.GetSelectedLevel(), seed).asPacket();
		SendToAll(packet);
	}
}

void GameServer::ResetLobby()
{
    m_in_game = false;
    m_world_sim.release();
    for (const PeerPtr& peer : m_peers)
    {
        if (peer->m_ready)
        {
            peer->m_player_data.lobby_ready = false;
            peer->m_player_controller.Reset();
        }
    }
}

//It is essential to set the sockets to non-blocking - m_socket.setBlocking(false)
//otherwise the server will hang waiting to read input from a connection
GameServer::RemotePeer::RemotePeer()
    : m_ready(false)
    , m_timed_out(false)
    , m_player_data()
    , m_player_controller()
{
    m_socket.setBlocking(false);
}
