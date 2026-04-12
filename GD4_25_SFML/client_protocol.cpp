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
	uint8_t characterId;
	char name[20];
	packet >> characterId;
	packet >> name;
	this->characterId = characterId;
	this->name = std::string(name);
}

sf::Packet ClientProtocol::IntroduceSelf::asPacket() const
{
	sf::Packet packet;
	packet << static_cast<uint8_t>(PacketType::kIntroduceSelf);
	packet << static_cast<uint8_t>(characterId);
	packet << name.substr(0, 20);
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
	char name[20];
	packet >> ready;
	packet >> this->characterId;
	packet >> name;
	this->name = std::string(name);
	isReady = ready != 0;
}

sf::Packet ClientProtocol::LobbyUpdateSelf::asPacket() const
{
	sf::Packet packet;
	packet << static_cast<uint8_t>(PacketType::kLobbyUpdateSelf);
	packet << static_cast<uint8_t>(isReady);
	packet << characterId;
	packet << name.substr(0, 20);
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