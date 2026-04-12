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
ServerProtocol::WelcomePlayer::WelcomePlayer(uint8_t assignedId, uint8_t assignedTeam, std::vector<PlayerData> playerList) :
	assignedId(assignedId), assignedTeam(assignedTeam), playerList(playerList) { }
ServerProtocol::WelcomePlayer::WelcomePlayer(sf::Packet& packet)
{
	packet >> this->assignedId;
	packet >> this->assignedTeam;
	// Deserialize player list
	uint8_t playerCount;
	packet >> playerCount;
	for (uint8_t i = 0; i < playerCount; ++i)
	{
		PlayerData player;
		char name[20];
		packet >> player.id;
		packet >> name;
		player.name = std::string(name);
		packet >> player.team;
		packet >> player.character;
		packet >> player.lobby_ready;
		playerList.push_back(player);
	}
}

sf::Packet ServerProtocol::WelcomePlayer::asPacket() const
{
	sf::Packet packet;
	packet << static_cast<uint8_t>(PacketType::kWelcomePlayer);
	packet << assignedId;
	packet << assignedTeam;
	packet << static_cast<uint8_t>(playerList.size());
	for (const auto& player : playerList)
	{
		packet << player.id;
		std::string name = player.name.substr(0, 20);
		packet << name;
		packet << player.team;
		packet << player.character;
		packet << player.lobby_ready;
	}
	return packet;
}


// --- Player Joined Packet ---
ServerProtocol::PlayerJoined::PlayerJoined(uint8_t playerId, uint8_t characterId, std::string& name, uint8_t team)
	: playerId(playerId), characterId(characterId), name(name), team(team) {}
ServerProtocol::PlayerJoined::PlayerJoined(sf::Packet& packet)
{
	char name[20];
	packet >> this->playerId;
	packet >> this->characterId;
	packet >> this->team;
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
	packet << team;
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


// --- Player Left Packet ---
ServerProtocol::LobbyPlayerUpdate::LobbyPlayerUpdate(uint8_t playerId, bool ready, uint8_t characterId, std::string name) :
	playerId(playerId), isReady(ready), characterId(characterId), name(name) {}
ServerProtocol::LobbyPlayerUpdate::LobbyPlayerUpdate(sf::Packet& packet)
{
	uint8_t ready;
	char name[20];
	packet >> this->playerId;
	packet >> ready;
	packet >> this->characterId;
	packet >> name;
	this->name = std::string(name);
	this->isReady = ready != 0;
}

sf::Packet ServerProtocol::LobbyPlayerUpdate::asPacket() const
{
	sf::Packet packet;
	packet << static_cast<uint8_t>(PacketType::kLobbyPlayerUpdate);
	packet << playerId;
	packet << static_cast<uint8_t>(isReady);
	packet << characterId;
	std::string name = this->name.substr(0, 20);
	packet << name;
	return packet;
}


// --- Player Left Packet ---
ServerProtocol::GameStart::GameStart(uint8_t levelId) : levelId(levelId) {}
ServerProtocol::GameStart::GameStart(sf::Packet& packet)
{
	packet >> this->levelId;
}

sf::Packet ServerProtocol::GameStart::asPacket() const
{
	sf::Packet packet;
	packet << static_cast<uint8_t>(PacketType::kGameStart);
	packet << levelId;
	return packet;
}
