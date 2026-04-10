/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#pragma once
#include "collider.hpp"

class PolygonCollider : public Collider
{
private:
	std::vector<sf::Vector2f> m_vertices;

public:
	PolygonCollider(float x, float y, std::vector<sf::Vector2f>& vertices, Physics* physics, PhysicsBody* body = nullptr, bool trigger = false);

	virtual bool CheckCollision(Collider* other) override;

	virtual bool CollideWith(Collider* other) override;
	virtual bool CollideWith(BoxCollider* other) override;
	virtual bool CollideWith(CircleCollider* other) override;
	virtual bool CollideWith(PolygonCollider* other) override;

	static sf::Vector2f ApproximateCenter(std::vector<sf::Vector2f>& vertices);
	static void ProjectVertices(sf::Vector2f& normal, std::vector<sf::Vector2f>& vertices, float& outMin, float& outMax);
	static bool CheckPolygonSAT(
		sf::Vector2f& firstPos, std::vector<sf::Vector2f>& firstVertices,
		sf::Vector2f& secondPos, std::vector<sf::Vector2f>& secondVertices,
	
		sf::Vector2f& outNormal, float& outDepth);

	static sf::Vector2f ClosestPolygonVertex(sf::Vector2f& from, sf::Vector2f& polyPos, std::vector<sf::Vector2f>& vertices);
	static bool CheckCircleSAT(
		sf::Vector2f& polyPos, std::vector<sf::Vector2f>& polyVertices,
		sf::Vector2f& circlePos, float circleRadius,
		sf::Vector2f& outNormal, float& outDepth);
};
