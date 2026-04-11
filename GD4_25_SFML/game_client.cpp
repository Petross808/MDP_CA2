/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#include <SFML/Network/Packet.hpp>

#include "game_client.hpp"
#include "game_server.hpp"
#include "client_protocol.hpp"

#include <iostream>

GameClient::GameClient() : m_status(ConnectionStatus::kNone), m_player_id(0), m_team(0)
{
	m_lobby_ready = false;
	m_socket.setBlocking(false);
}

GameClient::~GameClient()
{
	DisconnectFromServer();
}

void GameClient::Update(sf::Time dt)
{
	if(m_status == ConnectionStatus::kInProgress)
	{
		sf::Packet packet;
		auto status = m_socket.send(packet);
		if(status != sf::Socket::Status::Disconnected)
		{
			m_status = ConnectionStatus::kConnected;
			m_connection_timer.stop();
			OnConnected();
		}

		if(m_connection_timer.getElapsedTime().asSeconds() > 5.f)
		{
			m_status = ConnectionStatus::kTimeOut;
			m_connection_timer.stop();
		}
	}

	if (m_status == ConnectionStatus::kConnected)
	{
		// Keep Connection Alive
		sf::Packet keepAlivePacket = ClientProtocol::Empty().asPacket();
		auto status = m_socket.send(keepAlivePacket);
		if (status == sf::Socket::Status::Disconnected)
		{
			m_status = ConnectionStatus::kTimeOut;
		}

		sf::Packet packet;
		if (m_socket.receive(packet) == sf::Socket::Status::Done)
		{
			uint8_t packet_type;
			packet >> packet_type;
			HandlePacket(packet_type, packet);
		}
	}
}

ConnectionStatus GameClient::GetStatus() const
{
	return m_status;
}

void GameClient::ConnectToServer(std::optional<sf::IpAddress> ipAddress)
{
	if (ipAddress)
	{
		m_status = ConnectionStatus::kInProgress;
		auto _ = m_socket.connect(*ipAddress, GameServer::kServerPort, sf::seconds(5.f));
		m_connection_timer.restart();
	}
	else
	{
		m_status = ConnectionStatus::kInvalidIp;
		m_socket.disconnect();
	}
}

void GameClient::DisconnectFromServer()
{
	sf::Packet packet = ClientProtocol::LeaveGame().asPacket();
	SendPacket(packet);
	m_status = ConnectionStatus::kNone;
	m_socket.disconnect();
}

void GameClient::SendPacket(sf::Packet &packet)
{
	auto status = m_socket.send(packet);
}

void GameClient::ToggleLobbyReady()
{
	m_lobby_ready = !m_lobby_ready;
	sf::Packet packet = ClientProtocol::LobbyReady(m_lobby_ready).asPacket();
	SendPacket(packet);
}

bool GameClient::IsLobbyReady() const
{
	return m_lobby_ready;
}

void GameClient::OnConnected()
{
	m_lobby_ready = false;
}

void GameClient::HandlePacket(uint8_t packet_type, sf::Packet& packet)
{
	switch (static_cast<ServerProtocol::PacketType>(packet_type))
	{
		case ServerProtocol::PacketType::kEmpty:
		{
			ServerProtocol::Empty empty(packet);
			break;
		}
		case ServerProtocol::PacketType::kWelcomePlayer:
		{
			ServerProtocol::WelcomePlayer welcome(packet);
			m_player_id = welcome.assignedId;
			m_team = welcome.assignedTeam;

			std::cout << "Welcome! Your id is: " << std::to_string(welcome.assignedId) << std::endl;

			std::string name = "Player";
			sf::Packet packet = ClientProtocol::IntroduceSelf(0, name).asPacket();
			SendPacket(packet);;
			break;
		}
		case ServerProtocol::PacketType::kPlayerJoined:
		{
			ServerProtocol::PlayerJoined player_joined(packet);
			std::cout << "Player " << player_joined.name << " joined the game!" << std::endl;
			break;
		}
		case ServerProtocol::PacketType::kPlayerLeft:
		{
			ServerProtocol::PlayerLeft player_left(packet);
			std::cout << "Player " << static_cast<int>(player_left.playerId) << " left the game!" << std::endl;
			break;
		}
	}
}
