/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#include "box_collider.hpp"
#include "circle_collider.hpp"
#include "polygon_collider.hpp"

BoxCollider::BoxCollider(float x, float y, float width, float height, Physics* physics, PhysicsBody* body, bool trigger) :
	Collider(x, y, physics, body, trigger)
{
	m_box_size.x = width;
	m_box_size.y = height;
}

BoxCollider::~BoxCollider()
{
}

bool BoxCollider::CheckCollision(Collider* other)
{
	return other->CollideWith(this);
}

bool BoxCollider::CollideWith(Collider* other)
{
	return true;
}

bool BoxCollider::CollideWith(BoxCollider* other)
{
	sf::Vector2f min(GetWorldPosition());
	sf::Vector2 max(min + m_box_size);

	sf::Vector2 otherMin(other->GetWorldPosition());
	sf::Vector2 otherMax(otherMin + other->m_box_size);

	float minX = std::min(std::abs(max.x - otherMin.x), std::abs(otherMax.x - min.x));
	float minY = std::min(std::abs(max.y - otherMin.y), std::abs(otherMax.y - min.y));

	sf::Vector2f normal;
	float depth;

	if (minX < minY)
	{
		normal = { otherMin.x - min.x, 0 };
		depth = minX;
	}
	else
	{
		normal = { 0, otherMin.y - min.y };
		depth = minY;
	};

	normal = normal.normalized();

	ResolveCollision(other, normal, depth);

	return true;
}

bool BoxCollider::CollideWith(CircleCollider* other)
{
	return other->CollideWith(this);
}

bool BoxCollider::CollideWith(PolygonCollider* other)
{
	return other->CollideWith(this);
}

sf::Vector2f BoxCollider::GetBoxSize() const
{
	return m_box_size;
}