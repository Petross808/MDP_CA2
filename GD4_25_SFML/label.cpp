/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#include "label.hpp"
#include "button.hpp"
#include "utility.hpp"

gui::Label::Label(const std::string& text, const FontHolder& font) : m_text(font.Get(FontID::kMain), text, 16)
{
}

bool gui::Label::IsSelectable() const
{
    return false;
}


void gui::Label::SetText(const std::string& text, bool centre)
{
    m_text.setString(text);
    if(centre)
    {
        CentreText();
    }
}


void gui::Label::HandleEvent(const sf::Event& event)
{
}

void gui::Label::CentreText()
{
    Utility::CentreOrigin(m_text);
}

sf::Text& gui::Label::GetTextRef()
{
    return m_text;
}

void gui::Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_text, states);
}
