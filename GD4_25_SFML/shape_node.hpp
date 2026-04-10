/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#pragma once
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

#include <memory>
#include <vector>

#include "scene_node.hpp"


class ShapeNode : public SceneNode
{
public:
	ShapeNode(float width, float height);
	ShapeNode(float radius);
	ShapeNode(std::vector<sf::Vector2f>& vertices);
	~ShapeNode();

	void SetColor(sf::Color color);
	void SetTexture(sf::Texture &texture); //Jakub Polacek - GD4b - D00260171
	virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::unique_ptr<sf::Shape> m_shape;
	sf::FloatRect m_text_rect;
};