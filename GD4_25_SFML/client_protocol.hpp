/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#pragma once
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/Packet.hpp>

#include "e_action.hpp"

namespace ClientProtocol
{
	enum class PacketType
	{
		kEmpty,
		kIntroduceSelf,
		kLeaveGame,
		kLobbyUpdateSelf,
		kChangeLevel,
		kActionSelf
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
		uint8_t characterId;
	};

	struct LeaveGame
	{
	public:
		LeaveGame();
		LeaveGame(sf::Packet& packet);
		sf::Packet asPacket() const;
	};

	struct LobbyUpdateSelf
	{
	public:
		LobbyUpdateSelf(bool isReady, uint8_t characterId, std::string name);
		LobbyUpdateSelf(sf::Packet& packet);
		sf::Packet asPacket() const;
	public:
		bool isReady;
		uint8_t characterId;
		std::string name;
	};

	struct ChangeLevel
	{
	public:
		ChangeLevel(uint8_t levelId);
		ChangeLevel(sf::Packet& packet);
		sf::Packet asPacket() const;
	public:
		uint8_t levelId;
	};

	struct ActionSelf
	{
	public:
		ActionSelf(ActionID actionId, bool isPressed, bool isRealTime);
		ActionSelf(sf::Packet& packet);
		sf::Packet asPacket() const;
	public:
		ActionID actionId;
		bool isPressed;
		bool isRealTime;
	};
}

