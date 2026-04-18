/*
* Written by:
* Jakub Polacek - GD4b - D00260171
*/

#pragma once
#include "scene_node.hpp"
#include "physics.hpp"
#include "resource_identifiers.hpp"
#include "sound_player.hpp"
#include "game_data.hpp"
#include <random>

class Level
{
public:
	static void CreateBounds(SceneNode& root, Physics& physics, sf::FloatRect world_bounds, float wall_width, sf::Texture* texture);

	static void CreateClassic(SceneNode& root, Physics& physics, sf::FloatRect world_bounds, std::default_random_engine& rand, GameData& data, CommandQueue& command_queue, TextureHolder* texture_holder = nullptr, SoundPlayer* sounds = nullptr);
	static void CreateJagged(SceneNode& root, Physics& physics, sf::FloatRect world_bounds, std::default_random_engine& rand, GameData& data, CommandQueue& command_queue, TextureHolder* texture_holder = nullptr, SoundPlayer* sounds = nullptr);
	static void CreateDeadly(SceneNode& root, Physics& physics, sf::FloatRect world_bounds, std::default_random_engine& rand, GameData& data, CommandQueue& command_queue, TextureHolder* texture_holder = nullptr, SoundPlayer* sounds = nullptr);
};