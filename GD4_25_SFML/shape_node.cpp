/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

#include "shape_node.hpp"
#include <memory>

ShapeNode::ShapeNode(float width, float height) :
	m_shape(new sf::RectangleShape({ width, height })),
	m_text_rect({ 0, 0 }, { width, height })
{
}

ShapeNode::ShapeNode(float radius) :
	m_shape(new sf::CircleShape(radius)),
	m_text_rect({ 0, 0 }, { radius * 2.f , radius * 2.f })
{
}

ShapeNode::ShapeNode(std::vector<sf::Vector2f>& vertices)
{
	sf::ConvexShape* polygon = new sf::ConvexShape(vertices.size());

	for (auto i = 0; i < vertices.size(); i++)
	{
		polygon->setPoint(i, vertices[i]);
	}
	m_shape.reset(std::move(polygon));



	sf::Vector2f min(vertices[0]);
	sf::Vector2f max(vertices[0]);


	for (auto& vertex : vertices)
	{
		min.x = std::min(min.x, vertex.x);
		min.y = std::min(min.y, vertex.y);
		max.x = std::max(max.x, vertex.x);
		max.y = std::max(max.y, vertex.y);
	}

	m_text_rect.size = (max - min);

}

ShapeNode::~ShapeNode() = default;

void ShapeNode::SetColor(sf::Color color)
{
	m_shape->setOutlineColor(color);
	m_shape->setFillColor(color);
}

//Jakub Polacek - GD4b - D00260171
void ShapeNode::SetTexture(sf::Texture &texture)
{
	m_shape->setTexture(&texture);
	m_shape->setTextureRect({ {0,0},{ (int)m_text_rect.size.x, (int)m_text_rect.size.y } });
}

void ShapeNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_shape, states);
}
