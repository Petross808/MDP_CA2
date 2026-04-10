/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#include "text_node.hpp"
#include "utility.hpp"

TextNode::TextNode(const FontHolder& fonts, sf::String& text)
	: m_text(fonts.Get(FontID::kMain), text, 20)
{
}

void TextNode::SetString(const std::string& text)
{
	m_text.setString(text);
	Utility::CentreOrigin(m_text);
}

sf::Text& TextNode::GetTextRef()
{
	return m_text;
}

void TextNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_text, states);
}
