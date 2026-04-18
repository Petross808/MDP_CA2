/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#pragma once
#include "scene_node.hpp"
#include "command_queue.hpp"
#include "physics.hpp"
#include "game_data.hpp"
#include <random>

class WorldSimulation
{
public:
	explicit WorldSimulation(GameData& game_data);
	void Update(sf::Time dt);

	CommandQueue& GetCommandQueue();
	void SetSeed(uint64_t seed);
	Physics::PhysicsState GetPhysicsState() const;
	std::vector<std::pair<int, int>>& GetCollisionData();
	bool CheckScore();

	void SpawnPlayerPawn(int teamId, int playerId, int characterId, int numberOfPlayers);

private:
	void BuildScene();
	void HandleCollisions();

private:
	SceneNode m_scene_graph;
	sf::FloatRect m_world_bounds;
	CommandQueue m_command_queue;
	Physics m_physics;
	std::vector<std::pair<int, int>> m_collision_data;

	GameData& m_game_data;
	std::default_random_engine m_random;
};

