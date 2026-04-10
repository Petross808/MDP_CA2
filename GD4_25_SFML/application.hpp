/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#pragma once
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "player_controller.hpp"
#include "resource_holder.hpp"
#include "resource_identifiers.hpp"
#include "state_stack.hpp"
#include "music_player.hpp"
#include "sound_player.hpp"
#include "game_data.hpp"

class Application
{
public:
	Application();
	~Application();
	void Run();

private:
	void ProcessInput();
	void Update(sf::Time dt);
	void Render();


private:
	sf::RenderWindow m_window;
	PlayerController::Array m_players;
	GameData m_game_data;

	TextureHolder m_textures;
	FontHolder m_fonts;
	ShaderHolder m_shaders;

	MusicPlayer m_music;
	SoundPlayer m_sound;

	StateStack m_stack;
};

