/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
*/

#pragma once
#include <SFML/Graphics/Text.hpp>
#include "state.hpp"

class GameOverState : public State
{
public:
	GameOverState(StateStack& stack);
	virtual void Draw() override;
	virtual bool Update(sf::Time dt) override;
	virtual bool HandleEvent(const sf::Event& event);

private:
	sf::Text m_game_over_text;
	sf::Time m_elapsed_time;
};

