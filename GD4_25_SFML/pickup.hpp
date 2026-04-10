/*
* Written by:
* Jakub Polacek - GD4b - D00260171
*/

#pragma once
#include "scene_node.hpp"
#include "physics.hpp"
#include "e_pickup_id.hpp"


class Pickup : public SceneNode
{
public:
	Pickup(float x, float y, float radius, Physics* physics, PickupID pickup_id, sf::Texture* texture = nullptr);
	~Pickup();
	virtual void OnCollision(Collider& other, CommandQueue& command_queue) override;
	virtual bool IsDestroyed() const override;
private:
	PickupID m_pickup_id;
	bool m_is_destroyed;
	Command m_pickup_collect;
};