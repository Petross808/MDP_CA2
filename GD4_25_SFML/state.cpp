/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#include "state.hpp"
#include "state_stack.hpp"

State::State(StateStack& stack) : m_stack(&stack)
{
}

State::~State()
{
}

void State::RequestStackPop()
{
	m_stack->PopState();
}

void State::RequestStackClear()
{
	m_stack->ClearStack();
}

StateStack::Context State::GetContext() const
{
	return m_stack->GetContext();
}
