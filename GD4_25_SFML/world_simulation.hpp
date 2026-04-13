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

	void SpawnPlayerPawn(int teamId, int playerId, int characterId);

private:
	void BuildScene();
	sf::FloatRect GetViewBounds() const;
	void HandleCollisions();

private:
	SceneNode m_scene_graph;
	sf::FloatRect m_world_bounds;
	CommandQueue m_command_queue;
	Physics m_physics;

	GameData& m_game_data;
	std::default_random_engine m_random;
};

