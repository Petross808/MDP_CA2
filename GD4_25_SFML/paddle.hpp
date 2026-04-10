/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#pragma once
#include "pawn.hpp"
#include "physics.hpp"
#include "physics_body.hpp"
#include "sound_player.hpp"
#include "e_pickup_id.hpp"
#include "shape_node.hpp"
#include "command.hpp"

class Paddle : public Pawn
{
public:
	Paddle(int playerId, int characterId, float x, float y, Physics& physics, CommandQueue& command_queue,
		SoundPlayer& sounds, sf::Texture* texture = nullptr);
	void ApplyMove(float x, float y) override;
	void SetPickup(PickupID pickup_id); // Jakub Polacek - GD4b - D00260171
	void UsePickup() override; // Jakub Polacek - GD4b - D00260171

private:
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands) override;

private:
	float m_speed;
	sf::Vector2f m_move_vector;
	PhysicsBody m_physics_body;
	CommandQueue& m_command_queue;
	SoundPlayer& m_sounds;
	PickupID m_pickup_id;
	ShapeNode* m_shape;

	Command m_xFlip_pickup;
	Command m_yFlip_pickup;
	Command m_slow_pickup;
};

