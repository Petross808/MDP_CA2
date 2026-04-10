/*
* Written by:
* Jakub Polacek - GD4b - D00260171
*/

#pragma once
#include "scene_node.hpp"
#include "physics.hpp"
#include "e_pickup_id.hpp"
#include "Resource_identifiers.hpp"


class PickupSpawner : public SceneNode
{
public:
	PickupSpawner(float x, float y, int width, int height, Physics* physics_ptr, TextureHolder* texture_holder, float spawn_time = 5);
	~PickupSpawner();
protected:
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands) override;
private:
	Physics* m_physics_ptr;
	TextureHolder* m_texture_holder;
	int m_width;
	int m_height;
	float m_spawn_time;
	float m_cooldown;
};