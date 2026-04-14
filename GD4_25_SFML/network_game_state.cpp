/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#include "network_game_state.hpp"
#include "pause_state.hpp"
#include "game_over_state.hpp"

NetworkGameState::NetworkGameState(StateStack& stack) :
	State(stack),
	m_world(*GetContext().window, *GetContext().fonts, *GetContext().sound, *GetContext().shaders, *GetContext().game_data),
	m_players(*GetContext().players)
{
	GetContext().music->Play(MusicID::kGameMusic);
	GetContext().game_data->Reset();
	if (GetContext().game_data->GetSeed() != 0)
	{
		m_world.SetSeed(GetContext().game_data->GetSeed());
	}
	GetContext().client->SetGameState(this);
	
	for (auto& player : GetContext().client->GetPlayerList())
	{
		m_world.SpawnPlayerPawn(player.team, player.id, player.character);
	}
}

NetworkGameState::~NetworkGameState()
{
	GetContext().client->SetGameState(nullptr);
}

void NetworkGameState::Draw()
{
	m_world.Draw();
}

bool NetworkGameState::Update(sf::Time dt)
{
	m_world.Update(dt);

	if (GetContext().game_data->GetTeamOneScore() >= 3 ||
		GetContext().game_data->GetTeamTwoScore() >= 3)
	{
		RequestStackPush<GameOverState>();
	}

	return true;
}

bool NetworkGameState::HandleEvent(const sf::Event& event)
{
	if (const auto* key_pressed = event.getIf<sf::Event::KeyPressed>())
	{
		for (int i = 0; i < kActionCount; ++i)
		{
			Action& action = m_players[0].GetActionArray()[i];
			if(action.GetKeyBind() == key_pressed->scancode)
			{
				GetContext().client->DoActionOnRemote(action.GetID(), true, action.IsRealTime());
			}
		}
	}
	else if (const auto* key_released = event.getIf<sf::Event::KeyReleased>())
	{
		for (int i = 0; i < kActionCount; ++i)
		{
			Action& action = m_players[0].GetActionArray()[i];
			if (action.GetKeyBind() == key_released->scancode && action.IsRealTime())
			{
				GetContext().client->DoActionOnRemote(action.GetID(), false, true);
			}
		}
	}

	return true;
}

World& NetworkGameState::GetWorld()
{
	return m_world;
}


