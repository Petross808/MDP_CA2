/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#pragma once
#include <string>
#include "e_network_mode.hpp"

class GameData
{
public:
	GameData();
	~GameData();

	NetworkMode GetNetworkMode() const;
	int GetTeamOneScore() const;
	int GetTeamTwoScore() const;
	int GetSelectedLevel() const;
	int GetSelectedCharacter(int player_id) const;

	void SetNetworkMode(NetworkMode mode);
	void SetTeamOneScore(int score);
	void SetTeamTwoScore(int score);
	void SetSelectedLevel(int level);
	void SetSelectedCharacter(int player_id, int character);

	void CycleLevel();
	void CycleCharacter(int player_id);

	std::string GetLevelName() const;
	std::string GetCharacterName(int player_id);

	void Reset();

private:
	NetworkMode m_network_mode;

	int m_team_one_score;
	int m_team_two_score;
	int m_selected_level;
	int m_selected_player_one;
	int m_selected_player_two;
};

