/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#pragma once
#include <SFML/Graphics/Sprite.hpp>

#include "state_stack.hpp"
#include "container.hpp"
#include "button.hpp"
#include "label.hpp"

class SettingsState : public State
{
public:
	struct ButtonLabel
	{
		Action* m_action;
		gui::Button::Ptr m_button;
		gui::Label::Ptr m_label;

	public:
		ButtonLabel(Action* action, float x, float y, StateStack::Context& context, gui::Container& container);
	};

public:
	SettingsState(StateStack& stack);
	virtual void Draw() override;
	virtual bool Update(sf::Time dt) override;
	virtual bool HandleEvent(const sf::Event& event) override;

private:
	sf::Sprite m_background_sprite;
	gui::Container m_gui_container;
	std::vector<ButtonLabel> m_buttonLabel_vector;
};

