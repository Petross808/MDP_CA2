/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#include "state_stack.hpp"
#include "state.hpp"

StateStack::PendingChange::PendingChange(std::function<void()> pending_call) : callback(pending_call)
{
}

StateStack::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, ShaderHolder& shaders,
	PlayerController::Array& players, MusicPlayer& music, SoundPlayer& sound, GameData& game_data) :
	window(&window),
	textures(&textures),
	fonts(&fonts),
	shaders(&shaders),
	players(&players),
	music(&music),
	sound(&sound),
	game_data(&game_data)
{
}

StateStack::Context::~Context()
{
	window = nullptr;
	textures = nullptr;
	fonts = nullptr;
	shaders = nullptr;
	players = nullptr;
	music = nullptr;
	sound = nullptr;
}

StateStack::StateStack(Context context) : m_context(context)
{
}

StateStack::~StateStack() = default;

void StateStack::Update(sf::Time dt)
{
	for (auto itr = m_stack.rbegin(); itr != m_stack.rend(); ++itr)
	{
		if (!(*itr)->Update(dt))
		{
			break;
		}
	}
	ApplyPendingChanges();
}

void StateStack::Draw()
{
	for (State::Ptr& state : m_stack)
	{
		state->Draw();
	}

}

void StateStack::HandleEvent(const sf::Event& event)
{
	for (auto itr = m_stack.rbegin(); itr != m_stack.rend(); ++itr)
	{
		if (!(*itr)->HandleEvent(event))
		{
			break;
		}
	}
	ApplyPendingChanges();
}

void StateStack::PopState()
{
	std::function<void()> pop_func = [this]() { m_stack.pop_back(); };

	m_pending_list.emplace_back(PendingChange(pop_func));
}

void StateStack::ClearStack()
{
	std::function<void()> clear_func = [this]() { m_stack.clear(); };

	m_pending_list.emplace_back(PendingChange(clear_func));
}

void StateStack::ApplyPendingChanges()
{
	for (PendingChange change : m_pending_list)
	{
		change.callback();
	}

	m_pending_list.clear();
}

bool StateStack::IsEmpty() const
{
	return m_stack.empty();
}

StateStack::Context StateStack::GetContext() const
{
	return m_context;
}