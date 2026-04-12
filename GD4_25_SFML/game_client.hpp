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
#include "player_data.hpp"

class LobbyState;

class GameClient
{
public:
	typedef std::shared_ptr<GameClient> Ptr;

public:
	GameClient();
	~GameClient();

	void Start(LobbyState* lobby);
	void End();

	void Update(sf::Time dt);

	ConnectionStatus GetStatus() const;

	void ConnectToServer(std::optional<sf::IpAddress> ipAddress);
	void DisconnectFromServer();
	void SendPacket(sf::Packet &packet);

	void ToggleLobbyReady();

	PlayerData& GetLocalPlayer();
	PlayerData& GetPlayer(uint8_t playerId);

	void UpdatePlayerOnRemote();
	void ChangeLevelOnRemote(uint8_t levelId);

private:
	void HandlePacket(uint8_t packet_type, sf::Packet& packet);

private:
	bool m_client_running;
	ConnectionStatus m_status;
	sf::TcpSocket m_socket;
	sf::Clock m_connection_timer;

	PlayerData m_local_player;
	std::vector<PlayerData> m_player_list;

	LobbyState* m_lobby;
};
