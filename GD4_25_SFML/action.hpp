/*
* Written By:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#pragma once
#include <SFML/Window/Keyboard.hpp>
#include "command.hpp"
#include "e_action.hpp"

class Action
{
private:
	ActionID m_id;
	std::string m_name;
	bool m_realtime_action;
	sf::Keyboard::Scancode m_key_bind;
	Command m_command;
public:
	Action(ActionID id, std::string name, bool realtime, sf::Keyboard::Scancode key, Command command);
	void ChangeKeybind(sf::Keyboard::Scancode key_bind);

	std::string GetName() const;
	Command GetCommand() const;
	sf::Keyboard::Scancode GetKeyBind() const;
	bool IsRealTime() const;
	ActionID GetID() const;
};