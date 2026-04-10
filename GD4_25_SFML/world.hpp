/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#pragma once
#include <SFML/Graphics.hpp>

#include "resource_identifiers.hpp"
#include "scene_node.hpp"
#include "command_queue.hpp"
#include "sound_player.hpp"
#include "physics.hpp"
#include "game_data.hpp"

class World
{
public:
	explicit World(sf::RenderTarget& output_target, FontHolder& font, SoundPlayer& sounds, ShaderHolder& shaders, GameData & game_data);
	void Update(sf::Time dt);
	void Draw();

	CommandQueue& GetCommandQueue();

private:
	void LoadTextures();
	void BuildScene();
	sf::FloatRect GetViewBounds() const;
	void HandleCollisions();
	void UpdateSounds();

private:
	sf::RenderTarget& m_target;
	sf::RenderTexture m_scene_texture;
	sf::View m_camera;
	TextureHolder m_textures;
	FontHolder& m_fonts;
	SoundPlayer& m_sounds;
	ShaderHolder& m_shaders;
	SceneNode m_scene_graph;
	sf::FloatRect m_world_bounds;
	CommandQueue m_command_queue;
	Physics m_physics;
	
	GameData& m_game_data;
};

