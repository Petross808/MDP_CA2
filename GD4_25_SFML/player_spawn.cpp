/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#include "player_spawn.hpp"
#include "paddle.hpp"

#include <iostream>
#include "text_node.hpp"

PlayerSpawn::PlayerSpawn(int teamId, bool online, float x, float y, Physics& physics, CommandQueue& command_queue,
	SoundPlayer* sounds = nullptr, sf::Texture* texture = nullptr, FontHolder* fonts = nullptr) :
	SceneNode(ReceiverCategories::kPlayerSpawn),
	m_team_id(teamId),
	m_online(online),
	m_physics(physics),
	m_command_queue(command_queue),
	m_sounds(sounds),
	m_texture(texture),
	m_fonts(fonts)
{
	setPosition(sf::Vector2f(x, y));
}

int PlayerSpawn::GetTeamId() const
{
	return m_team_id;
}

void PlayerSpawn::SpawnPlayer(int playerId, int characterId, int numberOfPlayers, std::string name)
{
	std::cout << "Spawn Player " << playerId << std::endl;
	std::unique_ptr<Paddle> paddle(new Paddle(playerId, characterId, 0, 0,
		m_physics, m_command_queue, m_sounds, m_texture, m_online));
	paddle->UpdateByNumberOfPlayers(numberOfPlayers);
	if (m_fonts)
	{
		sf::String s(name);
		std::unique_ptr<TextNode> nameNode = std::make_unique<TextNode>(*m_fonts, s);
		nameNode->GetTextRef().setCharacterSize(48);
		nameNode->SetString(name);
		nameNode->setPosition(sf::Vector2f(0, 150));
		paddle->AttachChild(std::move(nameNode));
	}
	this->AttachChild(std::move(paddle));
}
