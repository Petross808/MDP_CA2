/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "physics.hpp"
#include "scene_node.hpp"
#include "physics_body.hpp"
#include "e_collision_layer.hpp"

class Physics;
class PhysicsBody;
class BoxCollider;
class CircleCollider;
class PolygonCollider;

class Collider : public SceneNode
{
private:
	Physics* m_physics;
	PhysicsBody* m_physics_body;
	bool m_is_trigger;
	CollisionLayer m_layer;
	unsigned int m_ignore_layers;

protected:
	sf::Vector2f m_box_size;

public:
	Collider(float x, float y, Physics* physics, PhysicsBody* body = nullptr, bool trigger = false);
	~Collider();
	virtual bool CheckCollision(Collider* other) = 0;

	virtual bool CollideWith(Collider* other) = 0;
	virtual bool CollideWith(BoxCollider* other) = 0;
	virtual bool CollideWith(CircleCollider* other) = 0;
	virtual bool CollideWith(PolygonCollider* other) = 0;

	bool IsDynamic() const;
	bool IsTrigger() const;
	CollisionLayer GetLayer() const;
	void SetLayer(CollisionLayer layer);
	void SetIgnoreLayers(unsigned int layers);

	bool CheckLayers(Collider* other) const;
	bool BoundingBoxOverlap(Collider* other);
	void ResolveCollision(Collider* other, sf::Vector2f& normal, float& depth);
};