/*
* Written by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#include "world_simulation.hpp"
#include "constants.hpp"
#include "level.hpp"
#include "score.hpp"
#include "player_spawn.hpp"

WorldSimulation::WorldSimulation(GameData& game_data)
	: m_scene_graph(ReceiverCategories::kScene)
	, m_world_bounds(sf::Vector2f(0.f, 0.f), sf::Vector2f(kWindowWidth, kWindowHeight))
	, m_physics()
	, m_collision_data()
	, m_game_data(game_data)
	, m_random((unsigned int)game_data.GetSeed())
{
	BuildScene();
}

void WorldSimulation::Update(sf::Time dt)
{
	m_physics.SimulateAllBodies(dt);
	HandleCollisions();

	//Process commands from the scenegraph
	while (!m_command_queue.IsEmpty())
	{
		m_scene_graph.OnCommand(m_command_queue.Pop(), dt);
	}

	m_scene_graph.RemoveWrecks();
	m_scene_graph.Update(dt, m_command_queue);
}

CommandQueue& WorldSimulation::GetCommandQueue()
{
	return m_command_queue;
}

void WorldSimulation::SetSeed(uint64_t seed)
{
	m_random.seed((unsigned int)seed);
}

Physics::PhysicsState WorldSimulation::GetPhysicsState() const
{
	return m_physics.GetPhysicsState();
}

std::vector<std::pair<int, int>>& WorldSimulation::GetCollisionData()
{
	return m_collision_data;
}

bool WorldSimulation::CheckScore()
{
	if (m_game_data.GetTeamOneScore() >= kPointsToWin ||
		m_game_data.GetTeamTwoScore() >= kPointsToWin)
	{
		return true;
	}
	return false;
}

void WorldSimulation::SpawnPlayerPawn(int teamId, int playerId, int characterId)
{
	Command spawn(DerivedAction<PlayerSpawn>(
		[this, teamId, playerId, characterId](PlayerSpawn& p, sf::Time dt)
		{
			if (p.GetTeamId() == teamId)
			{
				p.SpawnPlayer(playerId, characterId);
			}
		}
	), ReceiverCategories::kPlayerSpawn);
	m_command_queue.Push(spawn);
}

void WorldSimulation::BuildScene()
{
	switch (m_game_data.GetSelectedLevel())
	{
	case 0:
		Level::CreateClassic(m_scene_graph, m_physics, m_world_bounds, m_random, m_game_data, m_command_queue);
		break;
	case 1:
		Level::CreateJagged(m_scene_graph, m_physics, m_world_bounds, m_random, m_game_data, m_command_queue);
		break;
	case 2:
		Level::CreateDeadly(m_scene_graph, m_physics, m_world_bounds, m_random, m_game_data, m_command_queue);
		break;
	default:
		break;
	}

	std::unique_ptr<Score> score(new Score(m_game_data));
	m_scene_graph.AttachChild(std::move(score));
}

void WorldSimulation::HandleCollisions()
{
	std::vector<Physics::Pair> results;
	m_physics.EvaluateAllCollisions(results);

	m_collision_data.clear();
	for (auto& collision : results)
	{
		if (collision.first->ShouldReplicateCollisions() || collision.second->ShouldReplicateCollisions())
		{
			m_collision_data.emplace_back(collision.first->GetId(), collision.second->GetId());
		}
		collision.first->EvaluateCollision(*collision.second, m_command_queue);
		collision.second->EvaluateCollision(*collision.first, m_command_queue);
	}
}