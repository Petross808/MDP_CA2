/*
* Written by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#include "wall.hpp"
#include "shape_node.hpp"
#include "box_collider.hpp"
#include "circle_collider.hpp"
#include "polygon_collider.hpp"

Wall::Wall(float x, float y, float width, float height, Physics* physics, sf::Texture* texture)
{
	setPosition(sf::Vector2f(x, y));
	std::unique_ptr<Collider> collider = std::make_unique<BoxCollider>(0.f, 0.f, width, height, physics);
	collider->SetLayer(CollisionLayer::kWall);
	AttachChild(std::move(collider));

	std::unique_ptr<ShapeNode> shape(new ShapeNode(width, height));
	if (texture != nullptr)
	{
		shape->SetTexture(*texture);
	}
	AttachChild(std::move(shape));
}

Wall::Wall(float x, float y, float radius, Physics* physics, sf::Texture* texture)
{
	setPosition(sf::Vector2f(x, y));
	std::unique_ptr<Collider> collider = std::make_unique<CircleCollider>(0.f, 0.f, radius, physics);
	collider->SetLayer(CollisionLayer::kWall);
	AttachChild(std::move(collider));

	std::unique_ptr<ShapeNode> shape(new ShapeNode(radius));
	if (texture != nullptr)
	{
		shape->SetTexture(*texture);
	}
	AttachChild(std::move(shape));
}

Wall::Wall(float x, float y, std::vector<sf::Vector2f>& vertices, Physics* physics, sf::Texture* texture)
{
	setPosition(sf::Vector2f(x, y));
	std::unique_ptr<Collider> collider = std::make_unique<PolygonCollider>(0.f, 0.f, vertices, physics);
	collider->SetLayer(CollisionLayer::kWall);
	AttachChild(std::move(collider));

	std::unique_ptr<ShapeNode> shape(new ShapeNode(vertices));
	if (texture != nullptr)
	{
		shape->SetTexture(*texture);
	}
	AttachChild(std::move(shape));
}

Wall::~Wall() = default;