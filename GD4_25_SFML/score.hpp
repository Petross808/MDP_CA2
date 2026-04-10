/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#pragma once
#include "scene_node.hpp"
#include "resource_identifiers.hpp"
#include "game_data.hpp"

class Score : public SceneNode
{
public:
	Score(float x, float y, FontHolder& fonts, GameData& game_data);
	~Score();

	void IncrementTeamOne();
	void IncrementTeamTwo();

private:
	void UpdateText();

private:
	GameData& m_game_data;
	sf::Text* m_text;
	int m_team_one;
	int m_team_two;
};

