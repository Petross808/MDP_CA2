/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#pragma once
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/System/Clock.hpp>

#include <memory>

#include "e_connection_status.hpp"

class GameClient
{
public:
	typedef std::shared_ptr<GameClient> Ptr;

public:
	GameClient();
	~GameClient();

	void Update(sf::Time dt);

	ConnectionStatus GetStatus() const;

	void ConnectToServer(std::optional<sf::IpAddress> ipAddress);
	void DisconnectFromServer();
	void SendPacket(sf::Packet &packet);

	void ToggleLobbyReady();
	bool IsLobbyReady() const;

private:
	void OnConnected();
	void HandlePacket(uint8_t packet_type, sf::Packet& packet);

private:
	ConnectionStatus m_status;
	sf::TcpSocket m_socket;
	sf::Clock m_connection_timer;

	bool m_lobby_ready;
	uint8_t m_player_id;
	uint8_t m_team;
};

