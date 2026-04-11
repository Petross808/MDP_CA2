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


// --- Lobby Ready Packet ---
ClientProtocol::LobbyReady::LobbyReady(bool isReady) : isReady(isReady) {}
ClientProtocol::LobbyReady::LobbyReady(sf::Packet& packet)
{
	uint8_t ready;
	packet >> ready;
	isReady = ready != 0;
}

sf::Packet ClientProtocol::LobbyReady::asPacket() const
{
	sf::Packet packet;
	packet << static_cast<uint8_t>(PacketType::kLobbyReady);
	packet << static_cast<uint8_t>(isReady);
	return packet;
}