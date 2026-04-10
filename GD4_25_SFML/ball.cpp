/*
* Written by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#include "ball.hpp"
#include "shape_node.hpp"
#include "circle_collider.hpp"
#include "sound_node.hpp"
#include "utility.hpp"
#include "constants.hpp"

Ball::Ball(float x, float y, float radius, Physics* physics, sf::Texture* texture) :
	m_last_collided(),
	SceneNode(ReceiverCategories::kBall),
	m_physics_body(this, physics, 1, 1000, 0, 1.1f),
	m_bounce_sound(DerivedAction<SoundNode>([this](SoundNode& s, sf::Time dt) { s.PlaySound(SoundID::kBounce, GetWorldPosition(), 1 + Utility::RandomInt(-5,6) * 0.005f);}), ReceiverCategories::kSoundNode),
	m_start_delay(2),
	m_initial_pos(x, y),
	m_timer(m_start_delay),
	m_bounce_limit(),
	m_speed_limit(500.f)
{
	setPosition(m_initial_pos);
	std::unique_ptr<Collider> collider = std::make_unique<CircleCollider>(0.f, 0.f, radius, physics, &m_physics_body);
	collider->SetLayer(CollisionLayer::kBall);
	AttachChild(std::move(collider));

	std::unique_ptr<ShapeNode> shape(new ShapeNode(radius));
	if (texture != nullptr)
	{
		shape->SetTexture(*texture);
	}
	AttachChild(std::move(shape));
}

Ball::~Ball() = default;

void Ball::OnCollision(Collider& other, CommandQueue& command_queue)
{
	if (other.GetLayer() & CollisionLayer::kWall)
	{
		m_bounce_limit += 1;
	} 
	else if (other.GetLayer() & CollisionLayer::kPlayer)
	{
		Paddle* paddle = dynamic_cast<Paddle*>(other.GetParent());
		if (paddle != nullptr)
		{
			m_last_collided = paddle;
			m_bounce_limit = 0; 
			m_speed_limit += 50;
		}
	}

	if (other.GetLayer() != CollisionLayer::kPickup)
	{
		command_queue.Push(m_bounce_sound);
	}
	
	if (m_bounce_limit >= 10)
	{
		sf::Vector2f dir(Utility::RandomInt(100) - 50, Utility::RandomInt(100) - 50);
		m_physics_body.ApplyImpulse(10, dir);
		m_bounce_limit = 0;
	}
}

void Ball::GivePickup(PickupID pickup_id)
{
	if (m_last_collided != nullptr)
	{
		m_last_collided->SetPickup(pickup_id);
  }
}

void Ball::ResetBall()
{
	setPosition(m_initial_pos);
	m_physics_body.Reset();
	m_timer = m_start_delay;
	m_bounce_limit = 0;
	m_speed_limit = 500.f;
}

void Ball::MultiplyVelocity(float multX, float multY)
{
	auto vel = m_physics_body.GetVelocity();
	m_physics_body.SetVelocity(vel.x * multX, vel.y * multY);
}

void Ball::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	m_physics_body.ClampVelocity(m_speed_limit);

	if (m_timer <= 0) return;

	m_timer -= dt.asSeconds();

	if (m_timer <= 0)
	{
		sf::Vector2f dir(Utility::RandomInt(100) - 50, Utility::RandomInt(100) - 50);
		
		if (dir.y == 0)
		{
			dir.y = 5;
		}
		if (dir.x == 0)
		{
			dir.x = 20;
		}

		m_physics_body.ApplyImpulse(kInitialBallSpeed, dir);
	}
}
