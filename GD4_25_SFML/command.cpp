/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#include "command.hpp"

Command::Command() : action(), category(static_cast<unsigned int>(ReceiverCategories::kNone))
{
}

Command::Command(std::function<void(SceneNode&, sf::Time)> action, ReceiverCategories category) :
	action(action), category(static_cast<unsigned int>(category))
{
}
