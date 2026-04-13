/*
* Written by:
* Jakub Polacek - GD4b - D00260171
*/

#include "pickup_spawner.hpp"
#include "pickup.hpp"
#include "utility.hpp"

PickupSpawner::PickupSpawner(float x, float y, int width, int height, Physics* physics_ptr, std::default_random_engine& rand, SoundPlayer& sounds, TextureHolder& texture_holder, float spawn_time) :
	m_width(width),
	m_height(height),
	m_physics_ptr(physics_ptr),
	m_sounds(sounds),
	m_texture_holder(texture_holder),
	m_spawn_time(spawn_time),
	m_cooldown(spawn_time),
	m_random(rand)
{
	setPosition(sf::Vector2f(x, y));
}

PickupSpawner::~PickupSpawner() = default;


void PickupSpawner::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	if(m_cooldown > 0)
	{
		m_cooldown -= dt.asSeconds();
		return;
	}
	else
	{
		float x = static_cast<float>(Utility::RandomInt(0, m_width, m_random));
		float y = static_cast<float>(Utility::RandomInt(0, m_height, m_random));
		sf::Texture* water = &m_texture_holder.Get(TextureID::kWater);
		m_sounds.Play(SoundID::kPickupSpawn);

		int id = Utility::RandomInt(1, static_cast<int>(PickupID::kPickupCount), m_random);
		std::unique_ptr<Pickup> pickupTest(new Pickup(x, y, 30, m_physics_ptr, static_cast<PickupID>(id), water));
		this->AttachChild(std::move(pickupTest));
		m_cooldown = m_spawn_time;
		return;
	}
}