/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#include <SFML/Network/Packet.hpp>

#include "game_client.hpp"
#include "game_server.hpp"
#include "client_protocol.hpp"
#include "lobby_state.hpp"
#include "utility.hpp"
#include "network_game_state.hpp"
#include "pawn.hpp"

#include <iostream>

GameClient::GameClient() :
	m_status(ConnectionStatus::kNone), m_client_running(false),
	m_local_player(),
	m_player_list(0),
	m_lobby(nullptr),
	m_game_state(nullptr),
	m_network_controllers(0)
{
	m_socket.setBlocking(false);
}

GameClient::~GameClient()
{
	DisconnectFromServer();
}

void GameClient::Start(LobbyState* lobby)
{
	m_lobby = lobby;
	m_client_running = true;
	m_local_player.lobby_ready = false;
}

void GameClient::End()
{
	m_lobby = nullptr;
	m_client_running = false;
	DisconnectFromServer();
}

void GameClient::SetGameState(NetworkGameState* state)
{
	m_game_state = state;
}

void GameClient::Update(sf::Time dt)
{
	if (!m_client_running) return;

	if(m_status == ConnectionStatus::kInProgress)
	{
		sf::Packet packet;
		auto status = m_socket.send(packet);
		if(status != sf::Socket::Status::Disconnected)
		{
			m_status = ConnectionStatus::kConnected;
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
		sf::Packet keepAlivePacket = ClientProtocol::Empty().asPacket();
		auto status = m_socket.send(keepAlivePacket);
		if (status == sf::Socket::Status::Disconnected)
		{
			m_status = ConnectionStatus::kTimeOut;
			m_player_list.clear();
			if (m_lobby)
			{
				m_lobby->ClearPlayers();
			}
		}

		sf::Packet packet;
		if (m_socket.receive(packet) == sf::Socket::Status::Done)
		{
			uint8_t packet_type;
			packet >> packet_type;
			HandlePacket(packet_type, packet);
		}
	}

	if (m_game_state)
	{
		for (auto& controller : m_network_controllers)
		{
			controller->Update(m_game_state->GetWorld().GetCommandQueue());
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
	m_local_player.lobby_ready = !m_local_player.lobby_ready;
	UpdatePlayerOnRemote();
}

PlayerData& GameClient::GetLocalPlayer()
{
	return m_local_player;
}

PlayerData& GameClient::GetPlayer(uint8_t playerId)
{
	for (auto& player : m_player_list)
	{
		if (player.id == playerId)
		{
			return player;
		}
	}
	assert(false && "Player ID not found!"); // This should never happen if the server is working correctly
	return m_local_player; // Fallback to local player
}

std::vector<PlayerData>& GameClient::GetPlayerList()
{
	return m_player_list;
}

void GameClient::UpdatePlayerOnRemote()
{
	sf::Packet packet = ClientProtocol::LobbyUpdateSelf(m_local_player.lobby_ready, m_local_player.character, m_local_player.name).asPacket();
	SendPacket(packet);
}

void GameClient::ChangeLevelOnRemote(uint8_t levelId)
{
	sf::Packet packet = ClientProtocol::ChangeLevel(levelId).asPacket();
	SendPacket(packet);
}

void GameClient::DoActionOnRemote(ActionID actionId, bool isPressed, bool isRealTime)
{
	sf::Packet packet = ClientProtocol::ActionSelf(actionId, isPressed, isRealTime).asPacket();
	SendPacket(packet);
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
			m_local_player.id = welcome.assignedId;
			m_local_player.team = welcome.assignedTeam;
			m_player_list = welcome.playerList;

			std::cout << "Welcome! Your id is: " << std::to_string(welcome.assignedId) << std::endl;

			if (m_lobby)
			{
				for (auto& player : m_player_list)
				{
					m_lobby->AddPlayer(player);
				}
			}

			sf::Packet packet = ClientProtocol::IntroduceSelf(m_local_player.character, m_local_player.name).asPacket();
			SendPacket(packet);;
			break;
		}
		case ServerProtocol::PacketType::kPlayerJoined:
		{
			ServerProtocol::PlayerJoined player_joined(packet);
			std::cout << "Player " << player_joined.name << " joined the game!" << std::endl;

			PlayerData& data = m_player_list.emplace_back(
				player_joined.playerId,
				player_joined.name,
				player_joined.team,
				player_joined.characterId,
				false);

			if (m_lobby)
			{
				m_lobby->AddPlayer(data);
			}
			break;
		}
		case ServerProtocol::PacketType::kPlayerLeft:
		{
			ServerProtocol::PlayerLeft player_left(packet);
			std::cout << "Player " << static_cast<int>(player_left.playerId) << " left the game!" << std::endl;
			if (m_lobby)
			{
				m_lobby->RemovePlayer(player_left.playerId);
			}
			break;
		}
		case ServerProtocol::PacketType::kLobbyPlayerUpdate:
		{
			ServerProtocol::LobbyPlayerUpdate lobby_update(packet);
			PlayerData& player = GetPlayer(lobby_update.playerId);
			player.lobby_ready = lobby_update.isReady;
			player.character = lobby_update.characterId;
			player.name = lobby_update.name;
			if (m_lobby)
			{
				m_lobby->UpdatePlayer(player);
			}
			break;
		}
		case ServerProtocol::PacketType::kGameStart:
		{
			ServerProtocol::GameStart game_start(packet);
			std::cout << "Game is starting!" << std::endl;
			if(m_lobby)
			{
				for (auto& player : m_player_list)
				{
					auto cont = std::make_unique<NetworkController>();
					m_network_controllers.emplace_back(std::move(cont));
					m_network_controllers[m_network_controllers.size() - 1]->SetID(player.id);
				}
				m_lobby->StartGame(game_start.levelId, game_start.seed);
			}
			break;
		}
		case ServerProtocol::PacketType::kPhysicsSync:
		{
			ServerProtocol::PhysicsSync physics_sync(packet);
			if (m_game_state)
			{
				m_game_state->GetWorld().GetPhysics().ApplyPhysicsState(physics_sync.state);
			}
			break;
		}
		case ServerProtocol::PacketType::kPlayerUsePickup:
		{
			ServerProtocol::PlayerUsePickup playerPickup(packet);
			Command pickup(DerivedAction<Pawn>(
				[this, playerPickup](Pawn& p, sf::Time dt)
				{
					if (p.IsID(playerPickup.player_id)) p.UsePickup();
				}
			), ReceiverCategories::kPlayer);
			m_game_state->GetWorld().GetCommandQueue().Push(std::move(pickup));
			break;
		}
		case ServerProtocol::PacketType::kPlayerAction:
		{
			ServerProtocol::ActionPlayer action(packet);
			for (auto& controller : m_network_controllers)
			{
				if (controller->GetID() == action.playerId)
				{
					std::cout << "Apply input for " << action.playerId << std::endl;
					controller->ApplyNetworkInput(action.actionId, action.isPressed, action.isRealTime);
				}
			}
			break;
		}
	}
}