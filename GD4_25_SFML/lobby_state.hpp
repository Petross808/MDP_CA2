/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#pragma once
#include <SFML/Network/IpAddress.hpp>

#include "state.hpp"
#include "container.hpp"
#include "button.hpp"
#include "label.hpp"

class LobbyState : public State
{
public:
	LobbyState(StateStack& stack);
	~LobbyState();
	virtual void Draw() override;
	virtual bool Update(sf::Time dt) override;
	virtual bool HandleEvent(const sf::Event& event) override;

	void AddPlayer(PlayerData& player);
	void RemovePlayer(int id);
	void UpdatePlayer(PlayerData& player);
	void ClearPlayers();

private:
	void HandleIpInput(const sf::Event& event);
	void HandleNameInput(const sf::Event& event);

private:
	gui::Label::Ptr m_connection_label;
	gui::Button::Ptr m_ip_input;
	gui::Button::Ptr m_ready_button;
	gui::Button::Ptr m_name_input;

	gui::Container m_gui_container;

	std::map<uint8_t, gui::Label::Ptr> m_player_labels;
};