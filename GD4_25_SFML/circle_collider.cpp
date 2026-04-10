/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#include "box_collider.hpp"
#include "circle_collider.hpp"
#include "polygon_collider.hpp"
#include "utility.hpp"

CircleCollider::CircleCollider(float x, float y, float radius, Physics* physics, PhysicsBody* body, bool trigger) :
	Collider(x, y, physics, body, trigger),
	m_radius(radius)
{
	m_box_size.x = radius * 2;
	m_box_size.y = radius * 2;
}

bool CircleCollider::CheckCollision(Collider* other)
{
	return other->CollideWith(this);
}

bool CircleCollider::CollideWith(Collider* other)
{
	return true;
}

bool CircleCollider::CollideWith(BoxCollider* other)
{
	sf::Vector2f center(GetCenter());
	sf::Vector2f boxMin(other->GetWorldPosition());
	sf::Vector2f boxMax(boxMin + other->GetBoxSize());
	sf::Vector2f closest(std::clamp(center.x, boxMin.x, boxMax.x), std::clamp(center.y, boxMin.y, boxMax.y));

	sf::Vector2f offset(closest - center);
	float depth = m_radius - Utility::Length(offset);

	if (depth > 0)
	{
		if (depth == m_radius)
		{
			offset = center - (boxMin + (other->GetBoxSize() / 2.f));
		}
		sf::Vector2f normal(offset.normalized());
		ResolveCollision(other, normal, depth);
		return true;
	}

	return false;
}

bool CircleCollider::CollideWith(CircleCollider* other)
{
	float combinedRadius = m_radius + other->m_radius;

	sf::Vector2f offset(GetCenter() - other->GetCenter());
	float depth = combinedRadius - Utility::Length(offset);

	if (depth > 0)
	{
		sf::Vector2f normal(-offset.normalized());
		ResolveCollision(other, normal, depth);
		return true;
	}
	return false;
}

bool CircleCollider::CollideWith(PolygonCollider* other)
{
	return other->CollideWith(this);
}

float CircleCollider::GetRadius() const
{
	return m_radius;
}

sf::Vector2f CircleCollider::GetCenter() const
{
	return GetWorldPosition() + sf::Vector2f{m_radius, m_radius};
}
