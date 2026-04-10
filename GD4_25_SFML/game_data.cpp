/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#include "game_data.hpp"

GameData::GameData() : 
	m_team_one_score(0),
	m_team_two_score(0),
	m_selected_level(0),
	m_selected_player_one(),
	m_selected_player_two()
{
}

GameData::~GameData() = default;

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
		return "Diamonds";
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
		return "Hex";
	case 2:
		return "Diamond";
	default:
		return "Pro";
	}
}

void GameData::Reset()
{
	m_team_one_score = 0;
	m_team_two_score = 0;
}