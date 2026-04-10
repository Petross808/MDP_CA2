/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

#include <list>

#include "resource_holder.hpp"
#include "resource_identifiers.hpp"
#include "e_sound_id.hpp"


class SoundPlayer
{
public:
	SoundPlayer();

	void Play(SoundID effect);
	void Play(SoundID effect, sf::Vector2f position, float pitch = 1);

	void RemoveStoppedSounds();
	void SetListenerPosition(sf::Vector2f position);
	sf::Vector2f GetListenerPosition() const;


private:
	SoundBufferHolder m_sound_buffers;
	std::list<sf::Sound> m_sounds;
};

