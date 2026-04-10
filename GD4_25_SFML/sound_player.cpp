/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#include <SFML/Audio/Listener.hpp>

#include <cmath>

#include "sound_player.hpp"
#include "e_sound_id.hpp"

namespace
{
	// Sound coordinate system, point of view of a player in front of the screen:
	// X = left; Y = up; Z = back (out of the screen)
	const float ListenerZ = 300.f;
	const float Attenuation = 1.f;
	const float MinDistance2D = 200.f;
	const float MinDistance3D = std::sqrt(MinDistance2D * MinDistance2D + ListenerZ * ListenerZ);
}

SoundPlayer::SoundPlayer()
{
	m_sound_buffers.Load(SoundID::kButton, "Media/Sound/button_click.wav");
	m_sound_buffers.Load(SoundID::kBounce, "Media/Sound/ball_bounce.wav");
	m_sound_buffers.Load(SoundID::kPickupCollect, "Media/Sound/powerup_pickup.wav");
	m_sound_buffers.Load(SoundID::kPickupUse, "Media/Sound/powerup_use.wav");

	// Listener points towards the screen (default in SFML)
	sf::Listener::setDirection({ 0.f, 0.f, -1.f });
}

void SoundPlayer::Play(SoundID effect)
{
	Play(effect, GetListenerPosition());
}

void SoundPlayer::Play(SoundID effect, sf::Vector2f position, float pitch)
{
	m_sounds.emplace_back(m_sound_buffers.Get(effect));
	sf::Sound& sound = m_sounds.back();

	sound.setBuffer(m_sound_buffers.Get(effect));
	sound.setPosition({ position.x, -position.y, 0.f });
	sound.setAttenuation(Attenuation);
	sound.setMinDistance(MinDistance3D);
	sound.setVolume(100.f);
	sound.setPitch(pitch);

	sound.play();
}

void SoundPlayer::RemoveStoppedSounds()
{
	m_sounds.remove_if([](const sf::Sound& s)
		{
			return s.getStatus() == sf::Sound::Status::Stopped;
		});
}

void SoundPlayer::SetListenerPosition(sf::Vector2f position)
{
	sf::Listener::setPosition({ position.x, -position.y, ListenerZ });
}

sf::Vector2f SoundPlayer::GetListenerPosition() const
{
	sf::Vector3f position = sf::Listener::getPosition();
	return { position.x, -position.y };
}
