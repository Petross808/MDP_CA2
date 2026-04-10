/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>

#include "music_player.hpp"
#include "sound_player.hpp"
#include "player_controller.hpp"
#include "state_stack.hpp"

class State
{
public:
	typedef std::unique_ptr<State> Ptr;

public:
	State(StateStack& stack);
	virtual ~State();
	virtual void Draw() = 0;
	virtual bool Update(sf::Time dt) = 0;
	virtual bool HandleEvent(const sf::Event& event) = 0;

protected:
	template<typename T>
	void RequestStackPush();
	void RequestStackPop();
	void RequestStackClear();

	StateStack::Context GetContext() const;

private:
	StateStack* m_stack;
};

template<typename T>
void State::RequestStackPush()
{
	m_stack->PushState<T>();
}