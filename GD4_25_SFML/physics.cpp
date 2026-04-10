/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#include "physics.hpp"
#include "utility.hpp"

void Physics::CheckCollision(Collider* first, Collider* second, std::vector<Pair>& collisions)
{
	if (first->CheckLayers(second))
	{
		if (first->BoundingBoxOverlap(second))
		{
			if (first->CheckCollision(second))
			{
				collisions.emplace_back(Pair(first, second));
			}
		}
	}
}

void Physics::Register(Collider* shape)
{
	if (shape->IsDynamic())
	{
		m_dynamic_object_vector.emplace_back(shape);
	}
	else
	{
		m_static_object_vector.emplace_back(shape);
	}
}

void Physics::Unregister(Collider* shape)
{
	if (shape->IsDynamic())
	{
		auto size = m_dynamic_object_vector.size();
		for (int i = 0; i < size; i++)
		{
			if (shape == m_dynamic_object_vector[i])
			{
				std::swap(m_dynamic_object_vector[i], m_dynamic_object_vector[size - 1]);
				m_dynamic_object_vector.pop_back();
				return;
			}
		}
	}
	else
	{
		auto size = m_static_object_vector.size();
		for (int i = 0; i < size; i++)
		{
			if (shape == m_static_object_vector[i])
			{
				std::swap(m_static_object_vector[i], m_static_object_vector[size - 1]);
				m_static_object_vector.pop_back();
				return;
			}
		}
	}
}

void Physics::Register(PhysicsBody* body)
{
	m_physics_body_vector.emplace_back(body);
}

void Physics::Unregister(PhysicsBody* body)
{
	auto size = m_physics_body_vector.size();
	for (int i = 0; i < size; i++)
	{
		if (body == m_physics_body_vector[i])
		{
			std::swap(m_physics_body_vector[i], m_physics_body_vector[size - 1]);
			m_physics_body_vector.pop_back();
			return;
		}
	}
}

void Physics::EvaluateAllCollisions(std::vector<Pair>& result)
{
	std::vector<Collider*>::iterator inner;
	for (std::vector<Collider*>::iterator iter = m_dynamic_object_vector.begin(); iter != m_dynamic_object_vector.end(); iter++)
	{
		for (++(inner = iter); inner != m_dynamic_object_vector.end(); ++inner)
		{
			CheckCollision(*iter, *inner, result);
		}

		for (auto& static_obj : m_static_object_vector)
		{
			CheckCollision(*iter, static_obj, result);
		}
	}
}

void Physics::SimulateAllBodies(sf::Time dt)
{
	for (auto& body : m_physics_body_vector)
	{
		body->Simulate(dt);
	}
}
