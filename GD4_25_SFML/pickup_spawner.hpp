/*
* Written by:
* Jakub Polacek - GD4b - D00260171
*/

#pragma once
#include "scene_node.hpp"
#include "physics.hpp"
#include "e_pickup_id.hpp"
#include "resource_identifiers.hpp"
#include "sound_player.hpp"
#include <random>


class PickupSpawner : public SceneNode
{
public:
	PickupSpawner(float x, float y, int width, int height, bool auto_spawn, Physics* physics_ptr, std::default_random_engine& rand, SoundPlayer* sounds, TextureHolder* texture_holder);
	~PickupSpawner();
	void SpawnPickup();
protected:
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands) override;
private:
	Physics* m_physics_ptr;
	SoundPlayer* m_sounds;
	TextureHolder* m_texture_holder;
	int m_width;
	int m_height;
	bool m_auto_spawn;
	float m_spawn_time;
	float m_cooldown;
	std::default_random_engine& m_random;
};