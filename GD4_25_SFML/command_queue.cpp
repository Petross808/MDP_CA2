/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#include "command_queue.hpp"
#include "state_stack.hpp"

void CommandQueue::Push(const Command& command)
{
    m_queue.push(command);
}

Command CommandQueue::Pop()
{
    Command command = m_queue.front();
    m_queue.pop();
    return command;
}

bool CommandQueue::IsEmpty() const
{
    return m_queue.empty();
}


