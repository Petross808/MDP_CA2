/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#pragma once
#include <SFML/Audio/Music.hpp>

#include <map>
#include <string>

#include "resource_holder.hpp"
#include "resource_identifiers.hpp"
#include "e_music_id.hpp"

class MusicPlayer
{
public:
	MusicPlayer();

	void Play(MusicID theme);
	void Stop();

	void SetPaused(bool paused);
	void SetVolume(float volume);


private:
	sf::Music m_music;
	std::map<MusicID, std::string>	m_filenames;
	float m_volume;
};

