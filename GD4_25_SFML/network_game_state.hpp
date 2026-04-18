/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#pragma once
#include "state.hpp"
#include "world.hpp"

class NetworkGameState : public State
{
public:
	NetworkGameState(StateStack& stack);
	~NetworkGameState();
	virtual bool Update(sf::Time delta_time) override;
	virtual void Draw() override;
	virtual bool HandleEvent(const sf::Event& event) override;

	World& GetWorld();

private:
	World m_world;
	PlayerController::Array& m_players;
};

