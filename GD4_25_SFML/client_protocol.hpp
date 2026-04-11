#pragma once
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/Packet.hpp>

namespace ClientProtocol
{
	enum class PacketType
	{
		kEmpty,
		kIntroduceSelf,
		kLeaveGame,
		kLobbyReady,
	};

	struct Empty
	{
	public:
		Empty();
		Empty(sf::Packet& packet);
		sf::Packet asPacket() const;
	};


	struct IntroduceSelf
	{
	public:
		IntroduceSelf(uint8_t characterId, std::string& name);
		IntroduceSelf(sf::Packet& packet);
		sf::Packet asPacket() const;
	public:
		std::string name;
		int characterId;
	};

	struct LeaveGame
	{
		LeaveGame();
		LeaveGame(sf::Packet& packet);
		sf::Packet asPacket() const;
	};

	struct LobbyReady
	{
	public:
		LobbyReady(bool isReady);
		LobbyReady(sf::Packet& packet);
		sf::Packet asPacket() const;
	public:
		bool isReady;
	};
};

