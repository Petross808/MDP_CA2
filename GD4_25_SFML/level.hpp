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

class Level
{
public:
	static void CreateBounds(SceneNode& root, Physics& physics, sf::FloatRect world_bounds, float wall_width, sf::Texture* texture);

	static void CreateClassic(SceneNode& root, Physics& physics, TextureHolder& texture_holder, sf::FloatRect world_bounds, SoundPlayer& sounds, GameData& data, CommandQueue& command_queue);
	static void CreateDiamond(SceneNode& root, Physics& physics, TextureHolder& texture_holder, sf::FloatRect world_bounds, SoundPlayer& sounds, GameData& data, CommandQueue& command_queue);
};