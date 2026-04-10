/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#include "music_player.hpp"

MusicPlayer::MusicPlayer()
	: m_volume(50.f)
{
	m_filenames[MusicID::kMenuMusic] = "Media/Music/bgm_menu.ogg";
	m_filenames[MusicID::kGameMusic] = "Media/Music/bgm_battle_hype.ogg";
	m_music.setRelativeToListener(true);
}

void MusicPlayer::Play(MusicID theme)
{
	std::string filename = m_filenames[theme];

	if (!m_music.openFromFile(filename))
		throw std::runtime_error("Music " + filename + " could not be loaded.");

	m_music.setVolume(m_volume);
	m_music.setLooping(true);
	m_music.play();
}

void MusicPlayer::Stop()
{
	m_music.stop();
}

void MusicPlayer::SetVolume(float volume)
{
	m_volume = volume;
}

void MusicPlayer::SetPaused(bool paused)
{
	if (paused)
		m_music.pause();
	else
		m_music.play();
}
