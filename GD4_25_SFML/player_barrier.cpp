/*
* Written by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#include "player_barrier.hpp"
#include "box_collider.hpp"
#include "shape_node.hpp"

PlayerBarrier::PlayerBarrier(float x, float y, float width, float height, Physics* physics)
{
	setPosition(sf::Vector2f(x, y));
	std::unique_ptr<Collider> collider = std::make_unique<BoxCollider>(0.f, 0.f, width, height, physics);
	collider->SetLayer(CollisionLayer::kPlayerBarrier);
	collider->SetIgnoreLayers(CollisionLayer::kBall);
	AttachChild(std::move(collider));
}

PlayerBarrier::~PlayerBarrier() = default;
