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
		packet >> player.id;
		packet >> player.name;
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
		packet << player.name;
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
	packet >> this->playerId;
	packet >> this->characterId;
	packet >> this->team;
	packet >> this->name;
}

sf::Packet ServerProtocol::PlayerJoined::asPacket() const
{
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
	packet >> this->playerId;
	packet >> ready;
	packet >> this->characterId;
	packet >> this->name;
	this->isReady = ready != 0;
}

sf::Packet ServerProtocol::LobbyPlayerUpdate::asPacket() const
{
	sf::Packet packet;
	packet << static_cast<uint8_t>(PacketType::kLobbyPlayerUpdate);
	packet << playerId;
	packet << static_cast<uint8_t>(isReady);
	packet << characterId;
	packet << name;

	return packet;
}


// --- Game Start Packet ---
ServerProtocol::GameStart::GameStart(uint8_t levelId, uint64_t seed) : levelId(levelId), seed(seed) {}
ServerProtocol::GameStart::GameStart(sf::Packet& packet)
{
	packet >> this->levelId;
	packet >> this->seed;
}

sf::Packet ServerProtocol::GameStart::asPacket() const
{
	sf::Packet packet;
	packet << static_cast<uint8_t>(PacketType::kGameStart);
	packet << levelId;
	packet << seed;
	return packet;
}


// --- Physics Sync Packet ---
ServerProtocol::PhysicsSync::PhysicsSync(Physics::PhysicsState state) : state(state) {}
ServerProtocol::PhysicsSync::PhysicsSync(sf::Packet& packet)
{
	uint8_t size;
	packet >> size;

	int16_t xPos, yPos;
	int8_t	xVel, yVel;
	for (int i = 0; i < size; ++i)
	{
		packet >> xPos;
		packet >> yPos;
		packet >> xVel;
		packet >> yVel;

		state.positions.emplace_back(sf::Vector2f(xPos, yPos));
		state.velocities.emplace_back(sf::Vector2f(xVel*256, yVel*256));
	}
}

sf::Packet ServerProtocol::PhysicsSync::asPacket() const
{
	sf::Packet packet;
	packet << static_cast<uint8_t>(PacketType::kPhysicsSync);
	uint8_t size = static_cast<uint8_t>(state.positions.size());
	packet << size;
	
	for (int i = 0; i < size; ++i)
	{
		packet << static_cast<int16_t>((int)state.positions[i].x);
		packet << static_cast<int16_t>((int)state.positions[i].y);
		packet << static_cast<int8_t>((int)(state.velocities[i].x/256));
		packet << static_cast<int8_t>((int)(state.velocities[i].y/256));
	}

	return packet;
}


// --- Action Player Packet ---
ServerProtocol::ActionPlayer::ActionPlayer(uint8_t playerId, ActionID actionId, bool isPressed, bool isRealTime) :
	playerId(playerId), actionId(actionId), isPressed(isPressed), isRealTime(isRealTime) {
}
ServerProtocol::ActionPlayer::ActionPlayer(sf::Packet& packet)
{
	uint8_t actionId;
	packet >> this->playerId;
	packet >> actionId;
	packet >> this->isPressed;
	packet >> this->isRealTime;
	this->actionId = static_cast<ActionID>(actionId);
}

sf::Packet ServerProtocol::ActionPlayer::asPacket() const
{
	sf::Packet packet;
	packet << static_cast<uint8_t>(PacketType::kPlayerAction);
	packet << playerId;
	packet << static_cast<uint8_t>(actionId);
	packet << isPressed;
	packet << isRealTime;
	return packet;
}


// --- Collision Sync Packet ---
ServerProtocol::CollisionSync::CollisionSync(std::vector<std::pair<int, int>>& collisions) : collisions(collisions)
{
}
ServerProtocol::CollisionSync::CollisionSync(sf::Packet& packet)
{
	uint16_t size;
	packet >> size;

	uint8_t first;
	uint8_t second;
	for (uint16_t i = 0; i < size; i++)
	{
		packet >> first;
		packet >> second;
		collisions.emplace_back(first, second);
	}
}

sf::Packet ServerProtocol::CollisionSync::asPacket() const
{
	sf::Packet packet;
	packet << static_cast<uint8_t>(PacketType::kCollisionSync);
	
	uint16_t size = static_cast<uint16_t>(collisions.size());
	packet << size;
	
	for (uint16_t i = 0; i < size; i++)
	{
		packet << static_cast<uint8_t>(collisions[i].first);
		packet << static_cast<uint8_t>(collisions[i].second);
	}

	return packet;
}


// --- Spawn Pickup Packet ---
ServerProtocol::SpawnPickup::SpawnPickup() {}
ServerProtocol::SpawnPickup::SpawnPickup(sf::Packet& packet) {}

sf::Packet ServerProtocol::SpawnPickup::asPacket() const
{
	sf::Packet packet;
	packet << static_cast<uint8_t>(PacketType::kSpawnPickup);
	return packet;
}