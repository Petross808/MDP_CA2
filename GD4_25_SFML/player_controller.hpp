/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#pragma once
#include <SFML/Window/Event.hpp>

#include <map>

#include "command_queue.hpp"
#include "action.hpp"
#include "scene_node.hpp"
#include "constants.hpp"

class PlayerController
{
private:
	typedef sf::Keyboard::Scancode Key;
public:
	typedef std::array<PlayerController, kLocalPlayerCount> Array;

public:
	PlayerController(ReceiverCategories category, int player_id);
	void HandleEvent(const sf::Event& event, CommandQueue& command_queue);
	void HandleRealTimeInput(CommandQueue& command_queue);
	Action* GetActionArray();
	const Action* GetActionArray() const;

private:
	ReceiverCategories m_pawn_category;
	Action m_action_array[kActionCount];
	int m_id;
};

