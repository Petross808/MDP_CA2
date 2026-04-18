#include "network_controller.hpp"
#include "pawn.hpp"
#include "player_spawn.hpp"

NetworkController::NetworkController() :
    m_pawn_category(ReceiverCategories::kPlayer),
	m_id(-1),
	m_action_state(),
	m_action_array
    {
        Action(ActionID::kMoveUp, "Move Up", true, Key::W,
            Command(DerivedAction<Pawn>([this](Pawn& p, sf::Time dt) { if (p.IsID(m_id)) p.ApplyMove(0, -1); }), m_pawn_category)),
        Action(ActionID::kMoveDown, "Move Down", true, Key::S,
            Command(DerivedAction<Pawn>([this](Pawn& p, sf::Time dt) { if (p.IsID(m_id)) p.ApplyMove(0, 1); }), m_pawn_category)),
        Action(ActionID::kMoveLeft, "Move Left", true, Key::A,
            Command(DerivedAction<Pawn>([this](Pawn& p, sf::Time dt) { if (p.IsID(m_id)) p.ApplyMove(-1, 0); }), m_pawn_category)),
        Action(ActionID::kMoveRight, "Move Right", true, Key::D,
            Command(DerivedAction<Pawn>([this](Pawn& p, sf::Time dt) { if (p.IsID(m_id)) p.ApplyMove(1, 0); }), m_pawn_category)),
        Action(ActionID::kUsePickup, "Use Pickup", false, Key::Space,
            Command(DerivedAction<Pawn>([this](Pawn& p, sf::Time dt) { if (p.IsID(m_id)) p.UsePickup(); }), m_pawn_category))
    }
{
}

void NetworkController::ApplyNetworkInput(ActionID action, bool pressed, bool realtime)
{
    m_action_state[(int)action] = pressed;
}

void NetworkController::Update(CommandQueue& commandQueue)
{
    for (int i = 0; i < (int)ActionID::kActionCount; ++i)
    {
        if (m_action_state[i])
        {
            Action& action = m_action_array[i];
            if (!action.IsRealTime())
            {
                m_action_state[i] = false;
            }
            
            commandQueue.Push(std::move(action.GetCommand()));
        }
    }
}

int NetworkController::GetID() const
{
    return m_id;
}

void NetworkController::SetID(int id)
{
    m_id = id;
}

void NetworkController::Reset()
{
    m_action_state.fill(false);
}