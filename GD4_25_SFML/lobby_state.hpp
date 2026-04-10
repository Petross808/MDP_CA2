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
#include "game_client.hpp"
#include "game_server.hpp"

class LobbyState : public State
{
public:
	LobbyState(StateStack& stack);
	~LobbyState();
	virtual void Draw() override;
	virtual bool Update(sf::Time dt) override;
	virtual bool HandleEvent(const sf::Event& event) override;

private:
	gui::Label::Ptr m_connection_label;
	gui::Button::Ptr m_ip_input;
	gui::Button::Ptr m_ready_button;

	gui::Container m_gui_container;

	GameClient::Ptr m_game_client;
	GameServer::Ptr m_game_server;
};



