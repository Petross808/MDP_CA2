/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#include <SFML/Network/IpAddress.hpp>

#include <regex>

#include "lobby_state.hpp"
#include "button.hpp"
#include "menu_state.hpp"
#include "label.hpp"
#include "utility.hpp"
#include "network_game_state.hpp"


LobbyState::LobbyState(StateStack& stack) :
	State(stack),
	m_connection_label(nullptr),
	m_ip_input(nullptr),
	m_gui_container(),
	m_player_labels()
{
	GetContext().client->Start(this);
	auto data = GetContext().game_data;
	data->Reset();
	data->SetLevel(0);

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
		levelLabel->setPosition(sf::Vector2f(kWindowWidth / 2.f + 170, 300));
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
				GetContext().client->ChangeLevelOnRemote(data->GetSelectedLevel());
			});

		m_gui_container.Pack(levelLabel);
		m_gui_container.Pack(level);
		GetContext().server->Start();
		GetContext().client->ConnectToServer(sf::IpAddress::LocalHost);
	}

	if (data->GetNetworkMode() == NetworkMode::kClient)
	{
		auto autoJoin = std::make_shared<gui::Button>(GetContext());
		autoJoin->setPosition(sf::Vector2f(kWindowWidth / 2.f, 50));
		autoJoin->CentreButton();
		autoJoin->SetText("Quick Find Server");
		autoJoin->SetCallback([this]()
			{
				this->GetContext().client->FindServer();
			});
		m_gui_container.Pack(autoJoin);

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
				this->GetContext().client->ConnectToServer(ip);
			});
		m_gui_container.Pack(connectButton);
	}


	gui::Label::Ptr nameLabel = std::make_shared<gui::Label>("Player Name:", *GetContext().fonts);
	nameLabel->setPosition(sf::Vector2f(kWindowWidth / 2.f, 400));
	nameLabel->GetTextRef().setCharacterSize(20);
	nameLabel->CentreText();
	m_gui_container.Pack(nameLabel);

	m_name_input = std::make_shared<gui::Button>(GetContext());
	m_name_input->setPosition(sf::Vector2f(kWindowWidth / 2.f, 450));
	m_name_input->CentreButton();
	m_name_input->SetToggle(true);
	m_name_input->SetText(GetContext().client->GetLocalPlayer().name);
	m_gui_container.Pack(m_name_input);

	gui::Label::Ptr playerLabel = std::make_shared<gui::Label>("", *GetContext().fonts);
	playerLabel->setPosition(sf::Vector2f(kWindowWidth / 2.f + 170, 500));
	playerLabel->SetText(data->GetCharacterName(0), true);
	m_gui_container.Pack(playerLabel);

	auto player_one = std::make_shared<gui::Button>(GetContext());
	player_one->setPosition(sf::Vector2f(kWindowWidth / 2.f, 500));
	player_one->SetText("Character");
	player_one->CentreButton();
	player_one->SetCallback([this, playerLabel, data]()
		{
			data->CycleCharacter(0);
			playerLabel->SetText(data->GetCharacterName(0), true);
			GetContext().client->GetLocalPlayer().character = data->GetSelectedCharacter(0);
			GetContext().client->UpdatePlayerOnRemote();
		});
	m_gui_container.Pack(player_one);

	m_ready_button = std::make_shared<gui::Button>(GetContext());
	m_ready_button->setPosition(sf::Vector2f(kWindowWidth / 2.f, 575));
	m_ready_button->SetText("Ready Up");
	m_ready_button->CentreButton();
	m_ready_button->SetCallback([this]()
		{
			GetContext().client->ToggleLobbyReady();
		});
	m_gui_container.Pack(m_ready_button);

	auto back_button = std::make_shared<gui::Button>(GetContext());
	back_button->setPosition(sf::Vector2f(kWindowWidth / 2.f, 625));
	back_button->SetText("Quit to Menu");
	back_button->CentreButton();
	back_button->SetCallback([this]()
		{
			RequestStackPop();
			RequestStackPush<MenuState>();
		});
	m_gui_container.Pack(back_button);

	gui::Label::Ptr team_one_label = std::make_shared<gui::Label>("Team One", *GetContext().fonts);
	team_one_label->setPosition(sf::Vector2f(kWindowWidth / 4.f, 200));
	team_one_label->CentreText();
	m_gui_container.Pack(team_one_label);

	gui::Label::Ptr team_two_label = std::make_shared<gui::Label>("Team Two", *GetContext().fonts);
	team_two_label->setPosition(sf::Vector2f(3 * kWindowWidth / 4.f, 200));
	team_two_label->CentreText();
	m_gui_container.Pack(team_two_label);
}

LobbyState::~LobbyState()
{
	auto context = GetContext();
	context.client->End();
	context.server->End();
}

void LobbyState::Draw()
{
	sf::RenderWindow& window = *GetContext().window;
	window.draw(m_gui_container);

	for (const auto& playerLabel : m_player_labels)
	{
		window.draw(*playerLabel.second);
	}
}

bool LobbyState::Update(sf::Time dt)
{
	auto context = GetContext();
	context.client->Update(dt);
	
	if(context.client->GetLocalPlayer().lobby_ready)
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
		switch(context.client->GetStatus())
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

	if(context.client->GetStatus() == ConnectionStatus::kConnected)
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
	bool is_inputing_name = false;

	if (m_ip_input && m_ip_input->IsActive())
	{
		is_inputing_ip = true;
		HandleIpInput(event);
	}

	if(m_name_input && m_name_input->IsActive())
	{
		is_inputing_name = true;
		HandleNameInput(event);
	}

	if (!is_inputing_ip && !is_inputing_name)
	{
		m_gui_container.HandleEvent(event);
	}
	return false;
}

void LobbyState::HandleIpInput(const sf::Event& event)
{
	if (const auto* key_pressed = event.getIf<sf::Event::KeyReleased>())
	{
		if (
			key_pressed->scancode == sf::Keyboard::Scancode::Escape ||
			key_pressed->scancode == sf::Keyboard::Scancode::Enter)
		{
			m_ip_input->Deactivate();
			return;
		}
	}

	const auto* text_entered = event.getIf<sf::Event::TextEntered>();
	if (text_entered && text_entered->unicode < 128)
	{
		if (text_entered->unicode == 8) // Backspace
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
}

void LobbyState::HandleNameInput(const sf::Event& event)
{
	if (const auto* key_pressed = event.getIf<sf::Event::KeyReleased>())
	{
		if (
			key_pressed->scancode == sf::Keyboard::Scancode::Escape ||
			key_pressed->scancode == sf::Keyboard::Scancode::Enter)
		{
			m_name_input->Deactivate();
			GetContext().client->GetLocalPlayer().name = m_name_input->GetText();
			GetContext().client->UpdatePlayerOnRemote();
			return;
		}
	}

	const auto* text_entered = event.getIf<sf::Event::TextEntered>();
	if (text_entered && text_entered->unicode == 8) // Backspace
	{
		std::string current_text = m_name_input->GetText();
		if (!current_text.empty())
		{
			current_text.pop_back();
			m_name_input->SetText(current_text);
		}
	}
	if (text_entered && text_entered->unicode > 31 && text_entered->unicode < 128 && m_name_input->GetText().size() < 20)
	{
		char symbol = static_cast<char>(text_entered->unicode);
		std::string appended_name = m_name_input->GetText() + symbol;
		m_name_input->SetText(appended_name);
	}
}

void LobbyState::AddPlayer(PlayerData& player)
{
	gui::Label::Ptr label = std::make_shared<gui::Label>(player.name, *GetContext().fonts);

	int team_one = (int)std::count_if(m_player_labels.begin(), m_player_labels.end(), [&](auto& p)
		{ return p.second->getPosition().x < kWindowWidth/2.f; });
	int team_two = (int)m_player_labels.size() - team_one;
	int offset = (int)(player.team == 0 ? team_one : team_two) * 50;
	label->setPosition(sf::Vector2f((player.team == 0 ? 1 : 3) * kWindowWidth / 4.f, 250.f + offset));
	label->GetTextRef().setFillColor(player.lobby_ready ? sf::Color::Green : sf::Color::Red);
	label->GetTextRef().setCharacterSize(20);
	label->CentreText();

	m_player_labels.emplace(player.id, std::move(label));
}

void LobbyState::RemovePlayer(int id)
{
	auto label_itr = m_player_labels.find(id);
	float yPos = label_itr->second->getPosition().y;
	float xPos = label_itr->second->getPosition().x;
	if (label_itr != m_player_labels.end())
	{;
		m_player_labels.erase(label_itr);

		for (auto& label_pair : m_player_labels)
		{
			if (label_pair.second->getPosition().y > yPos && label_pair.second->getPosition().x == xPos)
			{
				label_pair.second->move(sf::Vector2f(0, -50));
			}
		}
	}
}

void LobbyState::UpdatePlayer(PlayerData& player)
{
	m_player_labels[player.id]->SetText(player.name, true);
	m_player_labels[player.id]->GetTextRef().setFillColor(player.lobby_ready ? sf::Color::Green : sf::Color::Red);
}

void LobbyState::ClearPlayers()
{
	m_player_labels.clear();
}

void LobbyState::StartGame(int levelId, uint64_t seed)
{
	GetContext().game_data->SetSeed(seed);
	GetContext().game_data->SetLevel(levelId);
	RequestStackPush<NetworkGameState>();
}

