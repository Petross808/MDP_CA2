/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#pragma once
#include <string>

struct PlayerData
{
public:
	PlayerData() : id(0), name("Player"), team(0), character(0), lobby_ready(false) {}
	PlayerData(uint8_t id, const std::string& name, uint8_t team, uint8_t character, bool lobby_ready)
		: id(id), name(name), team(team), character(character), lobby_ready(lobby_ready) {}
public:
	uint8_t id;
	std::string name;
	uint8_t team;
	uint8_t character;
	bool lobby_ready;
};

