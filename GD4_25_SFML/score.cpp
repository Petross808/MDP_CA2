/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#include "score.hpp"
#include "text_node.hpp"

Score::Score(float x, float y, FontHolder& fonts, GameData& game_data) : 
	SceneNode(ReceiverCategories::kScore),
	m_game_data(game_data),
	m_team_one(0),
	m_team_two(0)
{
	setPosition({ x, y });
	sf::String initText("0 : 0");
	std::unique_ptr<TextNode> textNode(new TextNode(fonts, initText));
	m_text = &textNode->GetTextRef();
	m_text->setCharacterSize(96);
	m_text->setStyle(sf::Text::Bold);
	m_text->setOrigin(m_text->getLocalBounds().getCenter());
	AttachChild(std::move(textNode));
}

Score::~Score() = default;

void Score::IncrementTeamOne()
{
	++m_team_one;
	m_game_data.SetTeamOneScore(m_team_one);
	UpdateText();
}

void Score::IncrementTeamTwo()
{
	++m_team_two;
	m_game_data.SetTeamTwoScore(m_team_two);
	UpdateText();
}

void Score::UpdateText()
{
	sf::String text(std::to_string(m_team_one) + " : " + std::to_string(m_team_two));
	m_text->setString(text);
	m_text->setOrigin(m_text->getLocalBounds().getCenter());
}
