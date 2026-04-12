/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#include "game_data.hpp"
#include "constants.hpp"

GameData::GameData() : 
	m_network_mode(NetworkMode::kLocal),
	m_team_one_score(0),
	m_team_two_score(0),
	m_selected_level(0),
	m_selected_player_one(),
	m_selected_player_two()
{
}

GameData::~GameData() = default;

NetworkMode GameData::GetNetworkMode() const
{
	return m_network_mode;
}

int GameData::GetTeamOneScore() const
{
	return m_team_one_score;
}

int GameData::GetTeamTwoScore() const
{
	return m_team_two_score;
}

int GameData::GetSelectedLevel() const
{
	return m_selected_level;
}

int GameData::GetSelectedCharacter(int player_id) const
{
	return player_id == 0 ? m_selected_player_one : m_selected_player_two;
}

void GameData::SetNetworkMode(NetworkMode mode)
{
	m_network_mode = mode;
}

void GameData::SetTeamOneScore(int score)
{
	m_team_one_score = score;
}

void GameData::SetTeamTwoScore(int score)
{
	m_team_two_score = score;
}

void GameData::SetSelectedLevel(int level)
{
	m_selected_level = level;
}

void GameData::SetSelectedCharacter(int player_id, int character)
{
	if (player_id == 0)
	{
		m_selected_player_one = character;
	}
	else
	{
		m_selected_player_two = character;
	}
}

std::string GameData::GetLevelName() const
{
	switch (m_selected_level)
	{
	case 0:
		return "Classic";
	case 1:
		return "Jagged";
	case 2:
		return "Deadly";
	default:
		return "Other";
	}
}

std::string GameData::GetCharacterName(int player_id)
{
	switch (player_id == 0 ? m_selected_player_one : m_selected_player_two)
	{
	case 0:
		return "Classic";
	case 1:
		return "Hexagon";
	case 2:
		return "Rhombus";
	case 3:
		return "Pro";
	default:
		return "Other";
	}
}

void GameData::CycleLevel()
{
	m_selected_level = (m_selected_level + 1) % kLevelCount;
}

void GameData::CycleCharacter(int player_id)
{
	SetSelectedCharacter(player_id, (GetSelectedCharacter(player_id) + 1) % kCharacterCount);
}

void GameData::Reset()
{
	m_team_one_score = 0;
	m_team_two_score = 0;
}