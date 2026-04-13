#include "player_spawn.hpp"
#include "paddle.hpp"

#include <iostream>

PlayerSpawn::PlayerSpawn(int teamId, bool online, float x, float y, Physics& physics, CommandQueue& command_queue,
	SoundPlayer* sounds = nullptr, sf::Texture* texture = nullptr) :
	SceneNode(ReceiverCategories::kPlayerSpawn),
	m_team_id(teamId),
	m_online(online),
	m_physics(physics),
	m_command_queue(command_queue),
	m_sounds(sounds),
	m_texture(texture)
{
	setPosition(sf::Vector2f(x, y));
}

int PlayerSpawn::GetTeamId() const
{
	return m_team_id;
}

void PlayerSpawn::SpawnPlayer(int playerId, int characterId)
{
	std::cout << "Spawn Player " << playerId << std::endl;
	std::unique_ptr<Paddle> paddle(new Paddle(playerId, characterId, 0, 0,
		m_physics, m_command_queue, m_sounds, m_texture, m_online));
	this->AttachChild(std::move(paddle));
}
