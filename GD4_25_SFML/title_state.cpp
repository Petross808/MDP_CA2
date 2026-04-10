/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#include "title_state.hpp"
#include "e_font_id.hpp"
#include "utility.hpp"
#include "menu_state.hpp"

TitleState::TitleState(StateStack& stack) :
    State(stack),
    m_show_text(true),
    m_text_effect_time(sf::Time::Zero),
    m_background_sprite(GetContext().textures->Get(TextureID::kTitleScreen)),
    m_text(GetContext().fonts->Get(FontID::kMain)),
    m_name(GetContext().fonts->Get(FontID::kMain))
{
    m_text.setString("Press any key to continue");
    Utility::CentreOrigin(m_text);
    m_text.setPosition(GetContext().window->getView().getSize() / 2.f);

    m_name.setString("ULTRA  PONG");
    m_name.setCharacterSize(108);
    m_name.setStyle(sf::Text::Bold);
    m_name.setFillColor(sf::Color::Yellow);
    Utility::CentreOrigin(m_name);
    m_name.setPosition(GetContext().window->getView().getSize() / 2.f);
    m_name.move({0, -200});

}

void TitleState::Draw()
{
    sf::RenderWindow& window = *GetContext().window;
    window.draw(m_background_sprite);

    if (m_show_text)
    {
        window.draw(m_text);
    }
    window.draw(m_name);
}

bool TitleState::Update(sf::Time dt)
{
    m_text_effect_time += dt;
    if (m_text_effect_time >= sf::seconds(0.5))
    {
        m_show_text = !m_show_text;
        m_text_effect_time = sf::Time::Zero;
    }
    return true;
}

bool TitleState::HandleEvent(const sf::Event& event)
{
    const auto* key_pressed = event.getIf<sf::Event::KeyReleased>();
    if (key_pressed)
    {
        RequestStackPop();
        RequestStackPush<MenuState>();
    }
    return false;
}
