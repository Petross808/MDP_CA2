/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#include <SFML/Network/Packet.hpp>

#include "game_client.hpp"
#include "game_server.hpp"

#include <iostream>

GameClient::GameClient() : m_status(ConnectionStatus::kNone)
{
	m_lobby_ready = false;
	m_socket.setBlocking(false);
}

GameClient::~GameClient()
{
}

void GameClient::Update(sf::Time dt)
{
	if(m_status == ConnectionStatus::kInProgress)
	{
		sf::Packet packet;
		auto status = m_socket.receive(packet);
		if(status != sf::Socket::Status::Disconnected)
		{
			m_status = ConnectionStatus::kConnected;
			m_lobby_ready = false;
			m_connection_timer.stop();
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
		sf::Packet packet;
		auto status = m_socket.send(packet);
		if (status == sf::Socket::Status::Disconnected)
		{
			m_status = ConnectionStatus::kTimeOut;
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