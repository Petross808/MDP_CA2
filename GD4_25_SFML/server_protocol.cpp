#include "server_protocol.hpp"

// --- Empty Packet ---
ServerProtocol::Empty::Empty() {}
ServerProtocol::Empty::Empty(sf::Packet& packet) {}

sf::Packet ServerProtocol::Empty::asPacket() const
{
	sf::Packet packet;
	packet << static_cast<uint8_t>(PacketType::kEmpty);
	return packet;
}


// --- Welcome Player Packet ---
ServerProtocol::WelcomePlayer::WelcomePlayer(uint8_t assignedId, uint8_t assignedTeam) :
	assignedId(assignedId), assignedTeam(assignedTeam) { }
ServerProtocol::WelcomePlayer::WelcomePlayer(sf::Packet& packet)
{
	packet >> this->assignedId;
	packet >> this->assignedTeam;
}
sf::Packet ServerProtocol::WelcomePlayer::asPacket() const
{
	sf::Packet packet;
	packet << static_cast<uint8_t>(PacketType::kWelcomePlayer);
	packet << assignedId;
	packet << assignedTeam;
	return packet;
}


// --- Player Joined Packet ---
ServerProtocol::PlayerJoined::PlayerJoined(uint8_t playerId, uint8_t characterId, std::string& name)
	: playerId(playerId), characterId(characterId), name(name) {}
ServerProtocol::PlayerJoined::PlayerJoined(sf::Packet& packet)
{
	char name[20];
	packet >> this->playerId;
	packet >> this->characterId;
	packet >> name;
	this->name = std::string(name);
}

sf::Packet ServerProtocol::PlayerJoined::asPacket() const
{
	std::string name = this->name.substr(0, 20);
	sf::Packet packet;
	packet << static_cast<uint8_t>(PacketType::kPlayerJoined);
	packet << playerId;
	packet << characterId;
	packet << name;
	return packet;
}


// --- Player Left Packet ---
ServerProtocol::PlayerLeft::PlayerLeft(uint8_t playerId) : playerId(playerId) {}
ServerProtocol::PlayerLeft::PlayerLeft(sf::Packet& packet)
{
	packet >> this->playerId;
}

sf::Packet ServerProtocol::PlayerLeft::asPacket() const
{
	sf::Packet packet;
	packet << static_cast<uint8_t>(PacketType::kPlayerLeft);
	packet << playerId;
	return packet;
}