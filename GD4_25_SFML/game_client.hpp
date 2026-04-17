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
#include "e_action.hpp"
#include "network_controller.hpp"

class LobbyState;
class NetworkGameState;

class GameClient
{
public:
	typedef std::shared_ptr<GameClient> Ptr;

public:
	GameClient();
	~GameClient();

	void Start(LobbyState* lobby);
	void End();

	void SetGameState(NetworkGameState* state);

	void Update(sf::Time dt);

	ConnectionStatus GetStatus() const;

	void ConnectToServer(std::optional<sf::IpAddress> ipAddress);
	void DisconnectFromServer();
	void SendPacket(sf::Packet &packet);

	void ToggleLobbyReady();

	PlayerData& GetLocalPlayer();
	PlayerData& GetPlayer(uint8_t playerId);
	std::vector<PlayerData>& GetPlayerList();

	void UpdatePlayerOnRemote();
	void ChangeLevelOnRemote(uint8_t levelId);
	void DoActionOnRemote(ActionID actionId, bool isPressed, bool isRealTime);

private:
	void HandlePacket(uint8_t packet_type, sf::Packet& packet);

private:
	bool m_client_running;
	ConnectionStatus m_status;
	sf::TcpSocket m_socket;
	sf::Clock m_connection_timer;
	sf::Time m_tick_timer;

	PlayerData m_local_player;
	std::vector<PlayerData> m_player_list;
	std::vector<std::unique_ptr<NetworkController>> m_network_controllers;

	LobbyState* m_lobby;
	NetworkGameState* m_game_state;
};
