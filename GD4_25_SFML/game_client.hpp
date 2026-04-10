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

public:

	bool m_lobby_ready;

private:
	ConnectionStatus m_status;
	sf::TcpSocket m_socket;
	sf::Clock m_connection_timer;
};

