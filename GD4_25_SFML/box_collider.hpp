/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#pragma once
#include "collider.hpp"

class BoxCollider : public Collider
{
public:
	BoxCollider(float x, float y, float width, float height, Physics* physics, PhysicsBody* body = nullptr, bool trigger = false);
	~BoxCollider();

	virtual bool CheckCollision(Collider* other) override;

	virtual bool CollideWith(Collider* other) override;
	virtual bool CollideWith(BoxCollider* other) override;
	virtual bool CollideWith(CircleCollider* other) override;
	virtual bool CollideWith(PolygonCollider* other) override;

	sf::Vector2f GetBoxSize() const;
};