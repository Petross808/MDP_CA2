/*
* Written by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#include "goal.hpp"
#include "box_collider.hpp"
#include "shape_node.hpp"
#include "score.hpp"
#include "ball.hpp"

Goal::Goal(int team, float x, float y, float width, float height, Physics* physics, sf::Texture* texture) :
	m_team(team),
	m_one_score(DerivedAction<Score>([](Score& s, sf::Time dt) { s.IncrementTeamOne(); }), ReceiverCategories::kScore),
	m_two_score(DerivedAction<Score>([](Score& s, sf::Time dt) { s.IncrementTeamTwo(); }), ReceiverCategories::kScore),
	m_reset_ball(DerivedAction<Ball>([](Ball& b, sf::Time dt) { b.ResetBall(); }), ReceiverCategories::kBall)
{
	setPosition(sf::Vector2f(x, y));
	std::unique_ptr<Collider> collider = std::make_unique<BoxCollider>(0.f, 0.f, width, height, physics, nullptr, true);
	collider->SetLayer(CollisionLayer::kGoal);
	collider->SetIgnoreLayers(CollisionLayer::kPlayer);
	AttachChild(std::move(collider));

	std::unique_ptr<ShapeNode> shape(new ShapeNode(width, height));
	if (texture != nullptr)
	{
		shape->SetTexture(*texture);
	}
	else
	{
		shape->SetColor(sf::Color::Magenta);
	}
	AttachChild(std::move(shape));
}

Goal::~Goal() = default;

void Goal::OnCollision(Collider& other, CommandQueue& command_queue)
{
	if (m_team == 0)
	{
		command_queue.Push(m_two_score);
	}
	else
	{
		command_queue.Push(m_one_score);
	}

	command_queue.Push(m_reset_ball);
}
