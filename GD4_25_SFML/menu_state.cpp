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

MenuState::MenuState(StateStack& stack) : State(stack), m_background_sprite(GetContext().textures->Get(TextureID::kTitleScreen))
{
    auto play_button = std::make_shared<gui::Button>(GetContext());
    play_button->setPosition(sf::Vector2f(700, 350));
    play_button->SetText("Play");
    play_button->SetCallback([this]()
        {
            RequestStackPush<LevelSelectState>();
        });

    auto settings_button = std::make_shared<gui::Button>(GetContext());
    settings_button->setPosition(sf::Vector2f(700, 400));
    settings_button->SetText("Settings");
    settings_button->SetCallback([this]()
        {
            RequestStackPush<SettingsState>();
        });

    auto exit_button = std::make_shared<gui::Button>(GetContext());
    exit_button->setPosition(sf::Vector2f(700, 450));
    exit_button->SetText("Exit");
    exit_button->SetCallback([this]()
        {
            RequestStackPop();
        });

    m_gui_container.Pack(play_button);
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

