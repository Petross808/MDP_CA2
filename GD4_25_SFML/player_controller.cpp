/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#include "player_controller.hpp"
#include "pawn.hpp"

PlayerController::PlayerController(ReceiverCategories pawn_category, int player_id) :
    m_id(player_id),
    m_pawn_category(pawn_category),
    m_action_array
    {
        Action("Move Up", true, Key::W,
            Command(DerivedAction<Pawn>([this](Pawn& p, sf::Time dt) { if (p.IsID(m_id)) p.ApplyMove(0, -1); }), m_pawn_category)),
        Action("Move Down", true, Key::S,
            Command(DerivedAction<Pawn>([this](Pawn& p, sf::Time dt) { if (p.IsID(m_id)) p.ApplyMove(0, 1); }), m_pawn_category)),
        Action("Move Left", true, Key::A,
            Command(DerivedAction<Pawn>([this](Pawn& p, sf::Time dt) { if (p.IsID(m_id)) p.ApplyMove(-1, 0); }), m_pawn_category)),
        Action("Move Right", true, Key::D,
            Command(DerivedAction<Pawn>([this](Pawn& p, sf::Time dt) { if (p.IsID(m_id)) p.ApplyMove(1, 0); }), m_pawn_category)),
        Action("Use Pickup", false, Key::Space,
            Command(DerivedAction<Pawn>([this](Pawn& p, sf::Time dt) { if (p.IsID(m_id)) p.UsePickup(); }), m_pawn_category))
    }
{
    if (player_id == 1)
    {
        m_action_array[0].ChangeKeybind(Key::Up);
        m_action_array[1].ChangeKeybind(Key::Down);
        m_action_array[2].ChangeKeybind(Key::Left);
        m_action_array[3].ChangeKeybind(Key::Right);
        m_action_array[4].ChangeKeybind(Key::Enter);
    }
}

void PlayerController::HandleEvent(const sf::Event& event, CommandQueue& command_queue)
{
    const auto* key_pressed = event.getIf<sf::Event::KeyPressed>();

    if (!key_pressed) return;

    for (const Action& action : m_action_array)
    {
        if (action.GetKeyBind() == key_pressed->scancode && !action.IsRealTime())
        {
            command_queue.Push(action.GetCommand());
        }
    }
}

void PlayerController::HandleRealTimeInput(CommandQueue& command_queue)
{
    for (const Action& action : m_action_array)
    {
        if (sf::Keyboard::isKeyPressed(action.GetKeyBind()) && action.IsRealTime())
        {
            command_queue.Push(action.GetCommand());
        }
    }
}

Action* PlayerController::GetActionArray()
{
    return m_action_array;
}

const Action* PlayerController::GetActionArray() const
{
    return m_action_array;
}
