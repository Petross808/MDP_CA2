/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#include "paddle.hpp"
#include "box_collider.hpp"
#include "circle_collider.hpp"
#include "polygon_collider.hpp"
#include "shape_node.hpp"
#include "sound_node.hpp"
#include "text_node.hpp"
#include "utility.hpp"
#include "ball.hpp"

Paddle::Paddle(int playerId, int characterId, float x, float y, Physics& physics, CommandQueue& command_queue, SoundPlayer& sounds, sf::Texture* texture) :
	Pawn(playerId),
	m_speed(5000),
	m_move_vector(),
	m_physics_body(this, &physics, 10.f, 500.f, 80.f, 0.5f, 0.7f),
	m_command_queue(command_queue),
	m_sounds(sounds),
	m_pickup_id(PickupID::kNone),
	m_shape(nullptr),
	m_xFlip_pickup(DerivedAction<Ball>([this](Ball& b, sf::Time dt) { b.MultiplyVelocity(-1, 1); }), ReceiverCategories::kBall),
	m_yFlip_pickup(DerivedAction<Ball>([this](Ball& b, sf::Time dt) { b.MultiplyVelocity(1, -1); }), ReceiverCategories::kBall),
	m_slow_pickup(DerivedAction<Ball>([this](Ball& b, sf::Time dt) { b.MultiplyVelocity(0.5f, 0.5f); }), ReceiverCategories::kBall)
{
	std::vector<sf::Vector2f> polygon;
	switch (characterId)
	{
	case 0:
		polygon = {
			{ 0.f, 0.f },
			{ 0.f, -3.f },
			{ 1.f, -3.f },
			{ 1.f, 0.f }
		};
		break;
	case 1:
		polygon = {
			{0, 2}, 
			{1, 0.75f}, 
			{1, -0.75f}, 
			{0, -2}, 
			{-1, -0.75f}, 
			{-1, 0.75f} };
		m_physics_body.SetValues(10.f, 500.f, 75.f, 0.1f, 0.5f);
		break;
	case 2:
		polygon = {
			{ 0.f, 3.f },
			{ 1.f, 0.f },
			{ 0.f, -3.f },
			{ -1.f, 0.f }
		};
		m_physics_body.SetValues(10.f, 450.f, 60.f, 0.1f, 0.2f);
		break;
	case 3:
		polygon = {
			{ 0.f, 0.f },
			{ 0.f, -2.f },
			{ 0.5f, -2.f },
			{ 0.5f, 0.f }
		};
		m_physics_body.SetValues(10.f, 800.f, 100.f, 0.1f, 1.f);
		m_speed = 10000.f;
		break;
	default:
		break;
	}
	
	for (auto& vert : polygon)
	{
		vert *= 50.f;
	}

	sf::Vector2f center = Utility::GetPolygonBound(polygon).getCenter();

	setPosition(sf::Vector2f(x - center.x , y - center.y));
	m_physics_body.SetAsKinematic();

	std::unique_ptr<Collider> collider = std::make_unique<PolygonCollider>(0.f, 0.f, polygon, &physics, &m_physics_body);
	collider->SetLayer(CollisionLayer::kPlayer);
	collider->SetIgnoreLayers(CollisionLayer::kPlayer);
	AttachChild(std::move(collider));

	std::unique_ptr<ShapeNode> shape(new ShapeNode(polygon));
	m_shape = shape.get();
	if (texture != nullptr)
	{
		shape->SetTexture(*texture);
	}
	AttachChild(std::move(shape));
}

void Paddle::ApplyMove(float x, float y)
{
	m_move_vector.x += x;
	m_move_vector.y += y;
}

// Jakub Polacek - GD4b - D00260171
void Paddle::SetPickup(PickupID pickup_id)
{
	m_pickup_id = pickup_id;

	if (m_shape == nullptr) return;

	switch (pickup_id)
	{
	case PickupID::kNone:
		break;
	case PickupID::kSpeedBoost:
		m_shape->SetColor(sf::Color::Color(173,216,230));
		break;
	case PickupID::kXFlip:
		m_shape->SetColor(sf::Color::Yellow);
		break;
	case PickupID::kYFlip:
		m_shape->SetColor(sf::Color::Red);
		break;
	case PickupID::kSlow:
		m_shape->SetColor(sf::Color::Magenta);
		break;
	default:
		break;
	}
}

// Jakub Polacek - GD4b - D00260171
void Paddle::UsePickup()
{
	if (m_pickup_id == PickupID::kNone) return;

	switch (m_pickup_id)
	{
		case PickupID::kNone:
			break;
		case PickupID::kSpeedBoost:
		{
			if (m_move_vector != sf::Vector2f(0, 0))
			{
				m_physics_body.ApplyImpulse(30000, m_move_vector.normalized());
			}
			break;
		}
		case PickupID::kXFlip:
			m_command_queue.Push(m_xFlip_pickup);
			break;
		case PickupID::kYFlip:
			m_command_queue.Push(m_yFlip_pickup);
			break;
		case PickupID::kSlow:
			m_command_queue.Push(m_slow_pickup);
			break;
		default:
			break;
	}

	m_sounds.Play(SoundID::kPickupUse);
	m_pickup_id = PickupID::kNone;
	if (m_shape != nullptr)
	{
		m_shape->SetColor(sf::Color::White);
	}
}

void Paddle::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (m_move_vector != sf::Vector2f())
	{
		sf::Vector2f force = m_move_vector.normalized() * m_speed * m_physics_body.GetMass() - m_physics_body.GetDrag();
		m_physics_body.AddForce(force.x, force.y);
		m_move_vector = { 0, 0 };
	}
}


