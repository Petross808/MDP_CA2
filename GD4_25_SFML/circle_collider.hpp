/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#pragma once
#include "collider.hpp"

class CircleCollider : public Collider
{
private:
	float m_radius;

public:
	CircleCollider(float x, float y, float radius, Physics* physics, PhysicsBody* body = nullptr, bool trigger = false);

	virtual bool CheckCollision(Collider* other) override;

	virtual bool CollideWith(Collider* other) override;
	virtual bool CollideWith(BoxCollider* other) override;
	virtual bool CollideWith(CircleCollider* other) override;
	virtual bool CollideWith(PolygonCollider* other) override;

	float GetRadius() const;
	sf::Vector2f GetCenter() const;
};