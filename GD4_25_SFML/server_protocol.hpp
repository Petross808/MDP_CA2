#pragma once
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/Packet.hpp>

#include "player_data.hpp"

namespace ServerProtocol
{
	enum class PacketType
	{
		kEmpty,
		kWelcomePlayer,
		kPlayerJoined,
		kPlayerLeft,
		kLobbyPlayerUpdate,
		kGameStart,
	};

	struct Empty
	{
	public:
		Empty();
		Empty(sf::Packet& packet);
		sf::Packet asPacket() const;
	};

	struct WelcomePlayer
	{
	public:
		WelcomePlayer(uint8_t assignedId, uint8_t assignedTeam, std::vector<PlayerData> playerList);
		WelcomePlayer(sf::Packet& packet);
		sf::Packet asPacket() const;
	public:
		uint8_t assignedId;
		uint8_t assignedTeam;
		std::vector<PlayerData> playerList;
	};

	struct PlayerJoined
	{
	public:
		PlayerJoined(uint8_t playerId, uint8_t characterId, std::string& name, uint8_t team);
		PlayerJoined(sf::Packet& packet);
		sf::Packet asPacket() const;
	public:
		uint8_t playerId;
		uint8_t characterId;
		uint8_t team;
		std::string name;
	};

	struct PlayerLeft
	{
	public:
		PlayerLeft(uint8_t playerId);
		PlayerLeft(sf::Packet& packet);
		sf::Packet asPacket() const;
	public:
		uint8_t playerId;
	};

	struct LobbyPlayerUpdate
	{
	public:
		LobbyPlayerUpdate(uint8_t playerId, bool isReady, uint8_t characterId, std::string name);
		LobbyPlayerUpdate(sf::Packet& packet);
		sf::Packet asPacket() const;
	public:
		uint8_t playerId;
		bool isReady;
		uint8_t characterId;
		std::string name;
	};

	struct GameStart
	{
	public:
		GameStart(uint8_t levelId);
		GameStart(sf::Packet& packet);
		sf::Packet asPacket() const;
	public:
		uint8_t levelId;
	};
}