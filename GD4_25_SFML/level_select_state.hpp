/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#pragma once
#include "state.hpp"
#include "container.hpp"

class LevelSelectState : public State
{
public:
	LevelSelectState(StateStack& stack);
	~LevelSelectState();

	virtual void Draw() override;
	virtual bool Update(sf::Time dt) override;
	virtual bool HandleEvent(const sf::Event& event) override;

	void CycleLevel();
	void CycleCharacter(int player_id);

private:
	sf::Sprite m_background_sprite;
	gui::Container m_gui_container;
};

