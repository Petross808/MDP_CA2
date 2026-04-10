/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#pragma once
#include "scene_node.hpp"
#include "resource_identifiers.hpp"

class TextNode : public SceneNode
{
public:
	explicit TextNode(const FontHolder& fonts, sf::String& text);
	void SetString(const std::string& text);
	sf::Text& GetTextRef();

private:
	virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Text m_text;
};

