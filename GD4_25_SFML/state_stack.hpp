/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#pragma once
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <map>
#include <functional>

#include "player_controller.hpp"
#include "resource_identifiers.hpp"
#include "music_player.hpp"
#include "sound_player.hpp"
#include "game_data.hpp"

class State;

class StateStack
{
public:
	struct Context
	{
		Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, ShaderHolder& shaders,
			PlayerController::Array& players, MusicPlayer& music, SoundPlayer& sound, GameData & data);
		~Context();

		sf::RenderWindow* window;
		TextureHolder* textures;
		FontHolder* fonts;
		ShaderHolder* shaders;
		PlayerController::Array* players;
		MusicPlayer* music;
		SoundPlayer* sound;
		GameData* game_data;
	};

public:
	explicit StateStack(Context context);
	~StateStack();
	void Update(sf::Time dt);
	void Draw();
	void HandleEvent(const sf::Event& event);

	template<typename T>
	void PushState();
	void PopState();
	void ClearStack();
	bool IsEmpty() const;

	Context GetContext() const;

private:
	void ApplyPendingChanges();

private:
	struct PendingChange
	{
		explicit PendingChange(std::function<void()> pending_call);
		std::function<void()> callback;
	};

private:
	std::list<std::unique_ptr<State>> m_stack;
	std::list<PendingChange> m_pending_list;
	Context m_context;
};

template<typename T>
void StateStack::PushState()
{
	static_assert(std::is_base_of<State, T>::value, "T must inherit State");

	std::function<void()> push_func = [this]() { m_stack.push_back(std::make_unique<T>(*this)); };

	m_pending_list.emplace_back(PendingChange(push_func));
}

