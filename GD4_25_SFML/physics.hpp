/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#pragma once
#include <vector>

#include "collider.hpp"
#include "physics_body.hpp"

class Collider;
class PhysicsBody;

class Physics
{
public:
	typedef std::pair<Collider*, Collider*> Pair;
private:
	std::vector<Collider*> m_dynamic_object_vector;
	std::vector<Collider*> m_static_object_vector;
	std::vector<PhysicsBody*> m_physics_body_vector;

private:
	void CheckCollision(Collider* first, Collider* second, std::vector<Pair>& collisions);
public:
	void Register(Collider* shape);
	void Unregister(Collider* shape);
	void Register(PhysicsBody* body);
	void Unregister(PhysicsBody* body);
	void EvaluateAllCollisions(std::vector<Pair>& result);
	void SimulateAllBodies(sf::Time dt);
};