/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#pragma once
#include "state.hpp"
#include "world.hpp"

class GameState : public State
{
public:
	GameState(StateStack& stack);
	virtual bool Update(sf::Time delta_time) override;
	virtual void Draw() override;
	virtual bool HandleEvent(const sf::Event& event) override;

private:
	World m_world;
	PlayerController::Array& m_players;
};

