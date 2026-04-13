#pragma once
#include <array>
#include "action.hpp"
#include "e_action.hpp"
#include "command_queue.hpp"

class NetworkController
{
private:
	typedef sf::Keyboard::Scancode Key;
public:
	NetworkController();
	void ApplyNetworkInput(ActionID action, bool pressed, bool realtime);
	void Update(CommandQueue& commandQueue);
	int GetID() const;
	void SetID(int id);
private:
	ReceiverCategories m_pawn_category;
	int m_id;
	std::array<bool, (int)ActionID::kActionCount> m_action_state;
	Action m_action_array[(int)ActionID::kActionCount];
};

