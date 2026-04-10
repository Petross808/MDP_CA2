/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#pragma once
#include <SFML/System/Time.hpp>
#include <functional>
#include "e_receiver_categories.hpp"

class SceneNode;

struct Command
{
	Command();
	Command(std::function<void(SceneNode&, sf::Time)> action, ReceiverCategories category);
	std::function<void(SceneNode&, sf::Time)> action;
	unsigned int category;
};

template<typename GameObject, typename Function>
std::function<void(SceneNode&, sf::Time)>
DerivedAction(Function fn)
{
	return [=](SceneNode& node, sf::Time dt)
		{
			//Check is the cast safe
			assert(dynamic_cast<GameObject*>(&node) != nullptr);
			//Downcast and invoke the function
			fn(static_cast<GameObject&>(node), dt);
		};
}

