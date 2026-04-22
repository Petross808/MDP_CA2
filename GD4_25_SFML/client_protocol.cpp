/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#include "client_protocol.hpp"

// --- Empty Packet ---
ClientProtocol::Empty::Empty() {}
ClientProtocol::Empty::Empty(sf::Packet& packet) {}

sf::Packet ClientProtocol::Empty::asPacket() const
{
	sf::Packet packet;
	packet << static_cast<uint8_t>(PacketType::kEmpty);
	return packet;
}


// --- Introduce Self Packet ---
ClientProtocol::IntroduceSelf::IntroduceSelf(uint8_t characterId, std::string& name) : characterId(characterId), name(name) {}
ClientProtocol::IntroduceSelf::IntroduceSelf(sf::Packet& packet)
{
	packet >> this->characterId;
	packet >> this->name;
}

sf::Packet ClientProtocol::IntroduceSelf::asPacket() const
{
	sf::Packet packet;
	packet << static_cast<uint8_t>(PacketType::kIntroduceSelf);
	packet << characterId;
	packet << name;
	return packet;
}


// --- Leave Game Packet ---
ClientProtocol::LeaveGame::LeaveGame() {}
ClientProtocol::LeaveGame::LeaveGame(sf::Packet& packet) {}

sf::Packet ClientProtocol::LeaveGame::asPacket() const
{
	sf::Packet packet;
	packet << static_cast<uint8_t>(PacketType::kLeaveGame);
	return packet;
}


// --- Lobby Update Self Packet ---
ClientProtocol::LobbyUpdateSelf::LobbyUpdateSelf(bool isReady, uint8_t characterId, std::string name) :
	isReady(isReady), characterId(characterId), name(name) {}
ClientProtocol::LobbyUpdateSelf::LobbyUpdateSelf(sf::Packet& packet)
{
	uint8_t ready;
	packet >> ready;
	packet >> this->characterId;
	packet >> this->name;
	isReady = ready != 0;
}

sf::Packet ClientProtocol::LobbyUpdateSelf::asPacket() const
{
	sf::Packet packet;
	packet << static_cast<uint8_t>(PacketType::kLobbyUpdateSelf);
	packet << static_cast<uint8_t>(isReady);
	packet << characterId;
	packet << name;
	return packet;
}


// --- Change Level Packet ---
ClientProtocol::ChangeLevel::ChangeLevel(uint8_t levelId) : levelId(levelId) {}
ClientProtocol::ChangeLevel::ChangeLevel(sf::Packet& packet)
{
	packet >> this->levelId;
}

sf::Packet ClientProtocol::ChangeLevel::asPacket() const
{
	sf::Packet packet;
	packet << static_cast<uint8_t>(PacketType::kChangeLevel);
	packet << this->levelId;
	return packet;
}


// --- Action Self Packet ---
ClientProtocol::ActionSelf::ActionSelf(ActionID actionId, bool isPressed, bool isRealTime) :
	actionId(actionId), isPressed(isPressed), isRealTime(isRealTime) {}
ClientProtocol::ActionSelf::ActionSelf(sf::Packet& packet)
{
	uint8_t actionId;
	packet >> actionId;
	packet >> this->isPressed;
	packet >> this->isRealTime;
	this->actionId = static_cast<ActionID>(actionId);
}

sf::Packet ClientProtocol::ActionSelf::asPacket() const
{
	sf::Packet packet;
	packet << static_cast<uint8_t>(PacketType::kActionSelf);
	packet << static_cast<uint8_t>(actionId);
	packet << isPressed;
	packet << isRealTime;
	return packet;
}
