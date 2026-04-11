#pragma once
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/Packet.hpp>

namespace ServerProtocol
{
	enum class PacketType
	{
		kEmpty,
		kWelcomePlayer,
		kPlayerJoined,
		kPlayerLeft,
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
		WelcomePlayer(uint8_t assignedId, uint8_t assignedTeam);
		WelcomePlayer(sf::Packet& packet);
		sf::Packet asPacket() const;
	public:
		uint8_t assignedId;
		uint8_t assignedTeam;
	};

	struct PlayerJoined
	{
	public:
		PlayerJoined(uint8_t playerId, uint8_t characterId, std::string& name);
		PlayerJoined(sf::Packet& packet);
		sf::Packet asPacket() const;
	public:
		uint8_t playerId;
		uint8_t characterId;
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
}