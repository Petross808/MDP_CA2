/*
* Written by:
* Petr Sulc - GD4b - D00261476
* Edited by:
* Jakub Polacek - GD4b - D00260171
*/

#include <SFML/Network/Packet.hpp>
#include <SFML/Network/UdpSocket.hpp>

#include "game_client.hpp"
#include "game_server.hpp"
#include "client_protocol.hpp"
#include "lobby_state.hpp"
#include "utility.hpp"
#include "network_game_state.hpp"
#include "pawn.hpp"

#include <iostream>
#include <fstream>

GameClient::GameClient() :
	m_status(ConnectionStatus::kNone), m_client_running(false),
	m_local_player(),
	m_player_list(0),
	m_lobby(nullptr),
	m_game_state(nullptr),
	m_network_controllers(0),
	m_tick_timer(sf::seconds(0))
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

	std::ifstream file("player_data.txt");

	if (file.is_open())
	{
		std::string data;
		getline(file, data);
		m_local_player.name = data;
		getline(file, data);
		int character = stoi(data);
		m_local_player.character = character;
		file.close();
		//find button
	}
}

void GameClient::End()
{
	//Save new player data
	std::ofstream file("player_data.txt", std::ofstream::trunc);
	file << m_local_player.name << std::endl;
	std::cout << "testing: " << static_cast<int>(m_local_player.character) << std::endl;
	file << static_cast<int>(m_local_player.character) << std::endl;
	file.close();


	//Cleanup
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
		m_tick_timer += dt;
		if (m_tick_timer.asSeconds() >= 0.5f)
		{
			m_tick_timer = sf::Time::Zero;
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

void GameClient::FindServer()
{
	sf::UdpSocket udpSocket;
	udpSocket.setBlocking(false);
	auto _ = udpSocket.bind(50002);
	std::string name = "Player";
	sf::Packet packet = ServerProtocol::Empty().asPacket();
	auto __ = udpSocket.send(packet, sf::IpAddress::Broadcast, 50001);

	sf::Packet response;
	std::optional<sf::IpAddress> senderAddress;
	unsigned short senderPort;
	
	sf::Clock searchTimer;
	sf::Socket::Status status = sf::Socket::Status::Disconnected;
	while (searchTimer.getElapsedTime() < sf::seconds(1.) && status != sf::Socket::Status::Done)
	{
		status = udpSocket.receive(response, senderAddress, senderPort);
		if (status == sf::Socket::Status::Done)
		{
			ConnectToServer(*senderAddress);
		}
	}
	
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
			auto found = std::find_if(m_player_list.begin(), m_player_list.end(), [&](PlayerData& p) {return p.id == player_left.playerId; });
			if (found != m_player_list.end())
			{
				m_player_list.erase(found);
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
				m_local_player.lobby_ready = false;
				for (auto& player : m_player_list)
				{
					player.lobby_ready = false;
					if (m_lobby)
					{
						m_lobby->UpdatePlayer(player);
					}
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
		case ServerProtocol::PacketType::kPlayerAction:
		{
			ServerProtocol::ActionPlayer action(packet);
			for (auto& controller : m_network_controllers)
			{
				if (controller->GetID() == action.playerId)
				{
					controller->ApplyNetworkInput(action.actionId, action.isPressed, action.isRealTime);
				}
			}
			break;
		}
		case ServerProtocol::PacketType::kCollisionSync:
		{
			ServerProtocol::CollisionSync collisionSync(packet);
			if (m_game_state)
			{
				CommandQueue& cq = m_game_state->GetWorld().GetCommandQueue();
				Physics& physics = m_game_state->GetWorld().GetPhysics();
				for (auto& collision : collisionSync.collisions)
				{
					physics.EvaluateCollisionById(collision.first, collision.second, cq);
				}
			}
			
			break;
		}
		default:
		{
			break;
		}
	}
}