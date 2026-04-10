/*
* Written by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#include "action.hpp"

Action::Action(std::string name, bool realtime, sf::Keyboard::Scancode key, Command command) :
	m_name(name), m_realtime_action(realtime), m_key_bind(key), m_command(command)
{
}

void Action::ChangeKeybind(sf::Keyboard::Scancode key_bind)
{
	m_key_bind = key_bind;
}

std::string Action::GetName() const
{
	return m_name;
}

Command Action::GetCommand() const
{
	return m_command;
}

sf::Keyboard::Scancode Action::GetKeyBind() const
{
	return m_key_bind;
}

bool Action::IsRealTime() const
{
	return m_realtime_action;
}
