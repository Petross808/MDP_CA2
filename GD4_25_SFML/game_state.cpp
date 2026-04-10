/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#include "game_state.hpp"
#include "pause_state.hpp"
#include "game_over_state.hpp"

GameState::GameState(StateStack& stack) :
	State(stack),
	m_world(*GetContext().window, *GetContext().fonts, *GetContext().sound, *GetContext().shaders, *GetContext().game_data),
	m_players(*GetContext().players)
{
	GetContext().music->Play(MusicID::kGameMusic);
	GetContext().game_data->Reset();
}

void GameState::Draw()
{
	m_world.Draw();
}

bool GameState::Update(sf::Time dt)
{
	m_world.Update(dt);

	auto context = GetContext();
	if (context.game_data->GetTeamOneScore() >= kPointsToWin || context.game_data->GetTeamTwoScore() >= kPointsToWin)
	{
		RequestStackPush<GameOverState>();
	}
	
	CommandQueue& commands = m_world.GetCommandQueue();

	for (auto& player : m_players)
	{
		player.HandleRealTimeInput(commands);
	}

	return true;
}

bool GameState::HandleEvent(const sf::Event& event)
{
	CommandQueue& commands = m_world.GetCommandQueue();

	for (auto& player : m_players)
	{
		player.HandleEvent(event, commands);
	}

	//Escape should bring up the pause menu
	const auto* keypress = event.getIf<sf::Event::KeyPressed>();
	if(keypress && keypress->scancode == sf::Keyboard::Scancode::Escape)
	{
		RequestStackPush<PauseState>();
	}
	return true;
}


