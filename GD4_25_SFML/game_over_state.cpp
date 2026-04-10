/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
*/

#include "game_over_state.hpp"
#include "utility.hpp"
#include "menu_state.hpp"
#include "constants.hpp"

GameOverState::GameOverState(StateStack& stack)
    : State(stack)
    , m_game_over_text(GetContext().fonts->Get(FontID::kMain))
    , m_elapsed_time()
{
    sf::Vector2f window_size(GetContext().window->getSize());

    auto context = GetContext();

    if (context.game_data->GetTeamOneScore() > context.game_data->GetTeamTwoScore())
    {
        m_game_over_text.setString("Player 1 wins!");
    }
    else
    {
        m_game_over_text.setString("Player 2 wins!");
    }
    
    m_game_over_text.setCharacterSize(70);
    Utility::CentreOrigin(m_game_over_text);
    m_game_over_text.setPosition(sf::Vector2f(0.5f * window_size.x, 0.4f * window_size.y));
}

void GameOverState::Draw()
{
    sf::RenderWindow& window = *GetContext().window;
    window.setView(window.getDefaultView());

    //Create a dark semi-transparent background
    sf::RectangleShape background_shape;
    background_shape.setFillColor(sf::Color(0, 0, 0, 150));
    background_shape.setSize(window.getView().getSize());

    window.draw(background_shape);
    window.draw(m_game_over_text);
}

bool GameOverState::Update(sf::Time dt)
{
    //Show gameover for 3 seconds and then return to the main menu
    m_elapsed_time += dt;
    if (m_elapsed_time > sf::seconds(kGameOverToMenuPause))
    {
        RequestStackClear();
        RequestStackPush<MenuState>();
    }
    return false;
}

bool GameOverState::HandleEvent(const sf::Event& event)
{
    return false;
}
