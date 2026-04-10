/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#include <SFML/Graphics/Text.hpp>

#include "menu_state.hpp"
#include "e_font_id.hpp"
#include "utility.hpp"
#include "button.hpp"
#include "game_state.hpp"
#include "level_select_state.hpp"
#include "settings_state.hpp"
#include "lobby_state.hpp"

MenuState::MenuState(StateStack& stack) : State(stack), m_background_sprite(GetContext().textures->Get(TextureID::kTitleScreen))
{
    auto play_button = std::make_shared<gui::Button>(GetContext());
    play_button->setPosition(sf::Vector2f(700, 350));
    play_button->SetText("Play Local");
    play_button->SetCallback([this]()
        {
            GetContext().game_data->SetNetworkMode(NetworkMode::kLocal);
            RequestStackPush<LevelSelectState>();
        });

    auto host_button = std::make_shared<gui::Button>(GetContext());
    host_button->setPosition(sf::Vector2f(700, 400));
    host_button->SetText("Host Online");
    host_button->SetCallback([this]()
        {
			RequestStackClear();
			GetContext().game_data->SetNetworkMode(NetworkMode::kServer);
            RequestStackPush<LobbyState>();
        });

    auto join_button = std::make_shared<gui::Button>(GetContext());
    join_button->setPosition(sf::Vector2f(700, 450));
    join_button->SetText("Join Online");
    join_button->SetCallback([this]()
        {
            RequestStackClear();
            GetContext().game_data->SetNetworkMode(NetworkMode::kClient);
            RequestStackPush<LobbyState>();
        });

    auto settings_button = std::make_shared<gui::Button>(GetContext());
    settings_button->setPosition(sf::Vector2f(700, 500));
    settings_button->SetText("Settings");
    settings_button->SetCallback([this]()
        {
            RequestStackPush<SettingsState>();
        });

    auto exit_button = std::make_shared<gui::Button>(GetContext());
    exit_button->setPosition(sf::Vector2f(700, 550));
    exit_button->SetText("Exit");
    exit_button->SetCallback([this]()
        {
            RequestStackPop();
        });

    m_gui_container.Pack(play_button);
    m_gui_container.Pack(host_button);
    m_gui_container.Pack(join_button);
    m_gui_container.Pack(settings_button);
    m_gui_container.Pack(exit_button);

    GetContext().music->Play(MusicID::kMenuMusic);
}

void MenuState::Draw()
{
    sf::RenderWindow& window = *GetContext().window;
    window.setView(window.getDefaultView());
    window.draw(m_background_sprite);
    window.draw(m_gui_container);
}

bool MenuState::Update(sf::Time dt)
{
    return true;
}

bool MenuState::HandleEvent(const sf::Event& event)
{
    m_gui_container.HandleEvent(event);
    return true;
}

