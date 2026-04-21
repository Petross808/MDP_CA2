#pragma once
#include "scene_node.hpp"
#include "sound_player.hpp"
#include "physics.hpp"


class PlayerSpawn : public SceneNode
{
public:
	PlayerSpawn(int teamId, bool online, float x, float y, Physics& physics, CommandQueue& command_queue,
		SoundPlayer* sounds, sf::Texture* texture, FontHolder* fonts);
	int GetTeamId() const;
	void SpawnPlayer(int playerId, int characterId, int numberOfPlayers, std::string name);

private:
	int m_team_id;
	bool m_online;
	Physics& m_physics;
	CommandQueue& m_command_queue;
	SoundPlayer* m_sounds;
	sf::Texture* m_texture;
	FontHolder* m_fonts;
};

