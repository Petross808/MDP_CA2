/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#include "box_collider.hpp"
#include "circle_collider.hpp"
#include "polygon_collider.hpp"
#include "utility.hpp"


PolygonCollider::PolygonCollider(float x, float y, std::vector<sf::Vector2f>& vertices, Physics* physics, PhysicsBody* body, bool trigger) :
	Collider(x, y, physics, body, trigger),
	m_vertices(vertices)
{
	sf::Vector2f min(m_vertices[0]);
	sf::Vector2f max(m_vertices[0]);


	for (auto& vertex : m_vertices)
	{
		min.x = std::min(min.x, vertex.x);
		min.y = std::min(min.y, vertex.y);
		max.x = std::max(max.x, vertex.x);
		max.y = std::max(max.y, vertex.y);
	}

	for (auto& vertex : m_vertices)
	{
		vertex -= min;
	}

	m_box_size = max - min;
	move(min);
}

bool PolygonCollider::CheckCollision(Collider* other)
{
	return other->CollideWith(this);
}

bool PolygonCollider::CollideWith(Collider* other)
{
	return true;
}

bool PolygonCollider::CollideWith(BoxCollider* other)
{
	sf::Vector2f firstPos(this->GetWorldPosition());
	sf::Vector2f secondPos(other->GetWorldPosition());

	sf::Vector2f boxSize(other->GetBoxSize());
	std::vector<sf::Vector2f> boxVertices
	{
		{0, 0},
		{boxSize.x, 0},
		boxSize,
		{0, boxSize.y}
	};

	sf::Vector2f normal;
	float depth;

	if (CheckPolygonSAT(firstPos, this->m_vertices, secondPos, boxVertices, normal, depth))
	{
		ResolveCollision(other, normal, depth);
		return true;
	}

	return false;
}

bool PolygonCollider::CollideWith(CircleCollider* other)
{
	sf::Vector2f polyPos(this->GetWorldPosition());
	sf::Vector2f circlePos(other->GetCenter());

	sf::Vector2f normal;
	float depth;

	if (CheckCircleSAT(polyPos, m_vertices, circlePos, other->GetRadius(), normal, depth))
	{
		ResolveCollision(other, normal, depth);
		return true;
	}

	return false;
}

bool PolygonCollider::CollideWith(PolygonCollider* other)
{
	sf::Vector2f firstPos(this->GetWorldPosition());
	sf::Vector2f secondPos(other->GetWorldPosition());

	sf::Vector2f normal;
	float depth;

	if (CheckPolygonSAT(firstPos, this->m_vertices, secondPos, other->m_vertices, normal, depth))
	{
		ResolveCollision(other, normal, depth);
		return true;
	}

	return false;
}

bool PolygonCollider::CheckPolygonSAT(
	sf::Vector2f& firstPos,
	std::vector<sf::Vector2f>& firstVertices,
	sf::Vector2f& secondPos,
	std::vector<sf::Vector2f>& secondVertices,
	sf::Vector2f& outNormal,
	float& outDepth)
{
	outNormal = sf::Vector2f();
	outDepth = std::numeric_limits<float>::max();

	for (size_t i = 0; i < firstVertices.size(); i++)
	{
		sf::Vector2f lineStart(firstVertices[i % firstVertices.size()]);
		sf::Vector2f lineEnd(firstVertices[(i + 1) % firstVertices.size()]);
		sf::Vector2f normal(-(lineEnd.y - lineStart.y), lineEnd.x - lineStart.x);
		normal = normal.normalized();

		float min1, max1, min2, max2;
		ProjectVertices(normal, firstVertices, min1, max1);
		ProjectVertices(normal, secondVertices, min2, max2);

		float offset = Utility::DotProduct(normal, firstPos - secondPos);
		min1 += offset;
		max1 += offset;

		if (min1 > max2 || min2 > max1)
		{
			return false;
		}

		float currentDepth = std::min(max2 - min1, max1 - min2);
		if (currentDepth < outDepth)
		{
			outDepth = currentDepth;
			outNormal = normal;
		}
	}

	for (size_t i = 0; i < secondVertices.size(); i++)
	{
		sf::Vector2f lineStart(secondVertices[i % secondVertices.size()]);
		sf::Vector2f lineEnd(secondVertices[(i + 1) % secondVertices.size()]);
		sf::Vector2f normal(-(lineEnd.y - lineStart.y), lineEnd.x - lineStart.x);
		normal = normal.normalized();

		float min1, max1, min2, max2;
		ProjectVertices(normal, secondVertices, min1, max1);
		ProjectVertices(normal, firstVertices, min2, max2);

		float offset = Utility::DotProduct(normal, secondPos - firstPos);
		min1 += offset;
		max1 += offset;

		if (min1 > max2 || min2 > max1)
		{
			return false;
		}

		float currentDepth = std::min(max2 - min1, max1 - min2);
		if (currentDepth < outDepth)
		{
			outDepth = currentDepth;
			outNormal = normal;
		}
	}

	outDepth = std::abs(outDepth / Utility::Length(outNormal));
	outNormal = outNormal.normalized();

	sf::Vector2f firstCenter(ApproximateCenter(firstVertices) + firstPos);
	sf::Vector2f secondCenter(ApproximateCenter(secondVertices) + secondPos);

	if (Utility::DotProduct(secondCenter - firstCenter, outNormal) < 0)
	{
		outNormal = -outNormal;
	}

	return true;
}

void PolygonCollider::ProjectVertices(sf::Vector2f& normal, std::vector<sf::Vector2f>& vertices, float& outMin, float& outMax)
{
	float dot;
	outMin = std::numeric_limits<float>::max();
	outMax = std::numeric_limits<float>::lowest();

	for (auto& vertex : vertices)
	{
		dot = Utility::DotProduct(normal, vertex);
		if (dot < outMin) outMin = dot;
		if (dot > outMax) outMax = dot;
	}
}

sf::Vector2f PolygonCollider::ApproximateCenter(std::vector<sf::Vector2f>& vertices)
{
	sf::Vector2f sum;
	for (auto& vertex : vertices)
	{
		sum += vertex;
	}
	return sum / (float)vertices.size();
}

sf::Vector2f PolygonCollider::ClosestPolygonVertex(sf::Vector2f& from, sf::Vector2f& polyPos, std::vector<sf::Vector2f>& vertices)
{
	sf::Vector2f closest;
	float minDist = std::numeric_limits<float>::max();

	sf::Vector2f vertOffset;
	float dist;
	for (auto& vert : vertices)
	{
		vertOffset = polyPos + vert - from;
		dist = Utility::SqrLength(vertOffset);
		if (dist < minDist)
		{
			closest = vert;
			minDist = dist;
		}
	}
	return closest;
}


bool PolygonCollider::CheckCircleSAT(
	sf::Vector2f& polyPos,
	std::vector<sf::Vector2f>& polyVertices,
	sf::Vector2f& circlePos,
	float circleRadius,
	sf::Vector2f& outNormal,
	float& outDepth)
{
	outNormal = sf::Vector2f();
	outDepth = std::numeric_limits<float>::max();

	for (size_t i = 0; i < polyVertices.size(); i++)
	{
		sf::Vector2f lineStart(polyVertices[i % polyVertices.size()]);
		sf::Vector2f lineEnd(polyVertices[(i + 1) % polyVertices.size()]);
		sf::Vector2f normal(-(lineEnd.y - lineStart.y), lineEnd.x - lineStart.x);
		normal = normal.normalized();

		float min1, max1;
		ProjectVertices(normal, polyVertices, min1, max1);
		float min2 =  -circleRadius;
		float max2 =  circleRadius;

		float offset = Utility::DotProduct(normal, polyPos - circlePos);
		min1 += offset;
		max1 += offset;

		if (min1 > max2 || min2 > max1)
		{
			return false;
		}
		
		float currentDepth = std::min(max2 - min1, max1 - min2);
		if (currentDepth < outDepth)
		{
			outDepth = currentDepth;
			outNormal = normal;
		}
		
	}

	sf::Vector2f normal(ClosestPolygonVertex(circlePos, polyPos, polyVertices) + polyPos - circlePos);
	normal = normal.normalized();

	float min1, max1;
	ProjectVertices(normal, polyVertices, min1, max1);
	float min2 = -circleRadius;
	float max2 = circleRadius;

	float offset = Utility::DotProduct(normal, polyPos - circlePos);
	min1 += offset;
	max1 += offset;

	if (min1 > max2 || min2 > max1)
	{
		return false;
	}

	float currentDepth = std::min(max2 - min1, max1 - min2);
	if (currentDepth < outDepth)
	{
		outDepth = currentDepth;
		outNormal = normal;
	}

	outDepth = std::abs(outDepth / Utility::Length(outNormal));
	outNormal = outNormal.normalized();

	sf::Vector2f polyCenter(ApproximateCenter(polyVertices) + polyPos);

	if (Utility::DotProduct(circlePos - polyCenter, outNormal) < 0)
	{
		outNormal = -outNormal;
	}
	
	return true;
}