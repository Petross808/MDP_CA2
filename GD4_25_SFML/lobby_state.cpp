/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#include <SFML/Network/IpAddress.hpp>

#include "lobby_state.hpp"
#include "button.hpp"
#include "menu_state.hpp"
#include "label.hpp"
#include "utility.hpp"
#include <regex>

LobbyState::LobbyState(StateStack& stack) :
	State(stack),
	m_connection_label(nullptr),
	m_ip_input(nullptr),
	m_gui_container(),
	m_game_server(nullptr)
{
	m_game_client = std::make_shared<GameClient>();
	auto data = GetContext().game_data;
	data->Reset();

	if(data->GetNetworkMode() == NetworkMode::kServer)
	{
		auto ip = sf::IpAddress::getLocalAddress();
		std::string ipString = ip ? ip->toString() : "Local IP: Not found";
		gui::Label::Ptr ipLabel = std::make_shared<gui::Label>("Local IP: " + ipString, *GetContext().fonts);
		ipLabel->setPosition(sf::Vector2f(kWindowWidth/2.f, 100));
		ipLabel->GetTextRef().setCharacterSize(48);
		ipLabel->CentreText();
		m_gui_container.Pack(ipLabel);

		gui::Label::Ptr levelLabel = std::make_shared<gui::Label>("", *GetContext().fonts);
		levelLabel->setPosition(sf::Vector2f(kWindowWidth / 2.f + 220, 300));
		levelLabel->SetText(data->GetLevelName(), true);
		m_gui_container.Pack(levelLabel);

		auto level = std::make_shared<gui::Button>(GetContext());
		level->setPosition(sf::Vector2f(kWindowWidth / 2.f, 300));
		level->SetText("Level");
		level->CentreButton();
		level->SetCallback([this, levelLabel, data]()
			{
				data->CycleLevel();
				levelLabel->SetText(data->GetLevelName(), true);
			});

		m_gui_container.Pack(levelLabel);
		m_gui_container.Pack(level);

		m_game_server = std::make_shared<GameServer>();
		m_game_client->ConnectToServer(sf::IpAddress::LocalHost);
	}

	if (data->GetNetworkMode() == NetworkMode::kClient)
	{
		gui::Label::Ptr ipLabel = std::make_shared<gui::Label>("Enter Server IP:", *GetContext().fonts);
		ipLabel->setPosition(sf::Vector2f(kWindowWidth/2.f, 100));
		ipLabel->GetTextRef().setCharacterSize(24);
		ipLabel->CentreText();
		m_gui_container.Pack(ipLabel);
		
		m_ip_input = std::make_shared<gui::Button>(GetContext());
		m_ip_input->setPosition(sf::Vector2f(kWindowWidth / 2.f, 150));
		m_ip_input->CentreButton();
		m_ip_input->SetToggle(true);
		m_ip_input->SetText("");
		m_gui_container.Pack(m_ip_input);

		m_connection_label = std::make_shared<gui::Label>("No Connection", *GetContext().fonts);
		m_connection_label->setPosition(sf::Vector2f(kWindowWidth / 2.f, 250));
		m_connection_label->GetTextRef().setCharacterSize(24);
		m_connection_label->GetTextRef().setFillColor(sf::Color::Red);
		m_connection_label->CentreText();
		m_gui_container.Pack(m_connection_label);

		auto connectButton = std::make_shared<gui::Button>(GetContext());
		connectButton->setPosition(sf::Vector2f(kWindowWidth / 2.f, 200));
		connectButton->CentreButton();
		connectButton->SetText("Connect");
		connectButton->SetCallback([this]()
			{
				auto ip = sf::IpAddress::resolve(m_ip_input->GetText());
				this->m_game_client->ConnectToServer(ip);
			});
		m_gui_container.Pack(connectButton);
	}

	gui::Label::Ptr playerLabel = std::make_shared<gui::Label>("", *GetContext().fonts);
	playerLabel->setPosition(sf::Vector2f(kWindowWidth / 2.f + 220, 375));
	playerLabel->SetText(data->GetCharacterName(0), true);
	m_gui_container.Pack(playerLabel);

	auto player_one = std::make_shared<gui::Button>(GetContext());
	player_one->setPosition(sf::Vector2f(kWindowWidth / 2.f, 375));
	player_one->SetText("Character");
	player_one->CentreButton();
	player_one->SetCallback([this, playerLabel, data]()
		{
			data->CycleCharacter(0);
			playerLabel->SetText(data->GetCharacterName(0), true);
		});
	m_gui_container.Pack(player_one);

	m_ready_button = std::make_shared<gui::Button>(GetContext());
	m_ready_button->setPosition(sf::Vector2f(kWindowWidth / 2.f, 500));
	m_ready_button->SetText("Ready Up");
	m_ready_button->CentreButton();
	m_ready_button->SetCallback([this]()
		{
			m_game_client->ToggleLobbyReady();
		});
	m_gui_container.Pack(m_ready_button);

	auto back_button = std::make_shared<gui::Button>(GetContext());
	back_button->setPosition(sf::Vector2f(kWindowWidth / 2.f, 550));
	back_button->SetText("Quit to Menu");
	back_button->CentreButton();
	back_button->SetCallback([this]()
		{
			RequestStackPop();
			RequestStackPush<MenuState>();
		});
	m_gui_container.Pack(back_button);
}

LobbyState::~LobbyState()
{
}

void LobbyState::Draw()
{
	sf::RenderWindow& window = *GetContext().window;
	window.draw(m_gui_container);
}

bool LobbyState::Update(sf::Time dt)
{
	m_game_client->Update(dt);
	
	if(m_game_client->IsLobbyReady())
	{
		m_ready_button->SetText("Cancel Ready");
	}
	else
	{
		m_ready_button->SetText("Ready Up");
	}

	if (m_connection_label)
	{
		m_ready_button->SetVisibility(false);
		switch(m_game_client->GetStatus())
		{
		case ConnectionStatus::kNone:
			m_connection_label->SetText("No Connection", true);
			m_connection_label->GetTextRef().setFillColor(sf::Color::Red);
			break;
		case ConnectionStatus::kInProgress:
			m_connection_label->SetText("Connecting...", true);
			m_connection_label->GetTextRef().setFillColor(sf::Color::Yellow);
			break;
		case ConnectionStatus::kConnected:
			m_connection_label->SetText("Connected!", true);
			m_connection_label->GetTextRef().setFillColor(sf::Color::Green);
			break;
		case ConnectionStatus::kTimeOut:
			m_connection_label->SetText("Connection Timed Out", true);
			m_connection_label->GetTextRef().setFillColor(sf::Color::Red);
			break;
		case ConnectionStatus::kInvalidIp:
			m_connection_label->SetText("Invalid IP Address", true);
			m_connection_label->GetTextRef().setFillColor(sf::Color::Red);
			break;
		default:
			break;
		}
	}

	if(m_game_client->GetStatus() == ConnectionStatus::kConnected)
	{
		m_ready_button->SetVisibility(true);
	}
	else
	{
		m_ready_button->SetVisibility(false);
	}

	return false;
}

bool LobbyState::HandleEvent(const sf::Event& event)
{
	bool is_inputing_ip = false;
	
	if (m_ip_input && m_ip_input->IsActive())
	{
		is_inputing_ip = true;
		const auto* text_entered = event.getIf<sf::Event::TextEntered>();
		if (text_entered && text_entered->unicode < 128)
		{
			if(text_entered->unicode == 8) // Backspace
			{
				std::string current_text = m_ip_input->GetText();
				if (!current_text.empty())
				{
					current_text.pop_back();
					m_ip_input->SetText(current_text);
				}
			}
			else
			{
				char symbol = static_cast<char>(text_entered->unicode);
				std::regex ip_regex("((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){0,3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)?");
				std::string appended_ip = m_ip_input->GetText() + symbol;
				if (std::regex_match(appended_ip, ip_regex))
				{
					m_ip_input->SetText(appended_ip);
				}
			}
		}

		if (const auto* key_pressed = event.getIf<sf::Event::KeyReleased>())
		{
			if(
				key_pressed->scancode == sf::Keyboard::Scancode::Escape ||
				key_pressed->scancode == sf::Keyboard::Scancode::Enter)
			{
				m_ip_input->Deactivate();
			}
		}

	}

	if (!is_inputing_ip)
	{
		m_gui_container.HandleEvent(event);
	}
	return false;
}
