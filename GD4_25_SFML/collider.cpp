/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#include "collider.hpp"
#include "utility.hpp"

Collider::Collider(float x, float y, Physics* physics, PhysicsBody* body, bool trigger) :
	m_physics(physics),
	m_physics_body(body),
	m_is_trigger(trigger),
	m_layer(),
	m_ignore_layers()
{
	m_physics->Register(this);
	setPosition({x, y});
}

Collider::~Collider()
{
	m_physics->Unregister(this);
}

bool Collider::CheckCollision(Collider* other)
{
	return other->CollideWith(this);
}

bool Collider::IsDynamic() const
{
	return m_physics_body != nullptr;
}

bool Collider::IsTrigger() const
{
	return m_is_trigger;
}

CollisionLayer Collider::GetLayer() const
{
	return m_layer;
}

void Collider::SetLayer(CollisionLayer layer)
{
	m_layer = layer;
}

void Collider::SetIgnoreLayers(unsigned int layers)
{
	m_ignore_layers = layers;
}

bool Collider::CheckLayers(Collider* other) const
{
	if (m_ignore_layers & other->m_layer || other->m_ignore_layers & m_layer)
	{
		return false;
	}

	return true;
}

bool Collider::BoundingBoxOverlap(Collider* other)
{
	sf::Vector2f min(GetWorldPosition());
	sf::Vector2 max(min + m_box_size);

	sf::Vector2 otherMin(other->GetWorldPosition());
	sf::Vector2 otherMax(otherMin + other->m_box_size);

	return (min.x <= otherMax.x && max.x >= otherMin.x && min.y <= otherMax.y && max.y >= otherMin.y);
}

void Collider::ResolveCollision(Collider* other, sf::Vector2f& normal, float& depth)
{
	if (m_is_trigger || other->m_is_trigger)
		return;

	float elasticity;
	sf::Vector2f relativeVelocity;
	float combinedMass;
	float strength;

	if (IsDynamic() && other->IsDynamic())
	{
		if (m_physics_body->IsKinematic() && !other->m_physics_body->IsKinematic())
		{
			other->m_physics_body->Move(normal * depth);
		}
		else if (!m_physics_body->IsKinematic() && other->m_physics_body->IsKinematic())
		{
			m_physics_body->Move(-normal * depth);
		}
		else
		{
			m_physics_body->Move(-normal * depth / 2.f);
			other->m_physics_body->Move(normal * depth / 2.f);
		}


		if (m_physics_body->IsKinematic() && other->m_physics_body->IsKinematic() ||
			!m_physics_body->IsKinematic() && !other->m_physics_body->IsKinematic())
		{
			elasticity = (m_physics_body->GetElasticity() + other->m_physics_body->GetElasticity()) / 2.f;
		}
		else if (m_physics_body->IsKinematic())
		{
			elasticity = other->m_physics_body->GetElasticity();
		}
		else
		{
			elasticity = m_physics_body->GetElasticity();
		}

		relativeVelocity = m_physics_body->GetVelocity() - other->m_physics_body->GetVelocity();
		float speedOnNormal = Utility::DotProduct(relativeVelocity, normal);

		if (-speedOnNormal >= 0.f) return;

		float firstMass = m_physics_body->IsKinematic() ? 0 : (1 / m_physics_body->GetMass());
		float secondMass = other->m_physics_body->IsKinematic() ? 0 : (1 / other->m_physics_body->GetMass());

		combinedMass = firstMass + secondMass ;
		strength = -(1 + elasticity) * speedOnNormal / (combinedMass == 0 ? 1 : combinedMass);

		sf::Vector2f gripAxis(-normal.y, normal.x);
		if (!m_physics_body->IsKinematic())
		{
			sf::Vector2f firstGrip = other->m_physics_body->GetVelocity().projectedOnto(gripAxis) * 0.001f * other->m_physics_body->GetGrip();
			m_physics_body->ApplyImpulse(strength, (normal + firstGrip).normalized());
		}
		if (!other->m_physics_body->IsKinematic())
		{
			sf::Vector2f secondGrip = m_physics_body->GetVelocity().projectedOnto(gripAxis) * 0.001f * m_physics_body->GetGrip();
			other->m_physics_body->ApplyImpulse(-strength, (normal + secondGrip).normalized());
		}
	}
	else if (IsDynamic())
	{
		m_physics_body->Move(-normal * depth);

		elasticity = m_physics_body->GetElasticity();
		relativeVelocity = m_physics_body->GetVelocity();
		combinedMass = 1 / m_physics_body->GetMass();
		strength = -(1 + elasticity) * Utility::DotProduct(relativeVelocity, normal) / combinedMass;
		m_physics_body->ApplyImpulse(strength, normal);
	}
	else
	{
		other->m_physics_body->Move(normal * depth);

		elasticity = other->m_physics_body->GetElasticity();
		relativeVelocity = other->m_physics_body->GetVelocity();
		combinedMass = 1 / other->m_physics_body->GetMass();
		strength = -(1 + elasticity) * Utility::DotProduct(relativeVelocity, normal) / combinedMass;
		other->m_physics_body->ApplyImpulse(strength, normal);
	}
}
