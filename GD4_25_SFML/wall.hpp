/*
* Written by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#pragma once
#include "scene_node.hpp"
#include "physics.hpp"

class Wall : public SceneNode
{ 
public:
	Wall(float x, float y, float width, float height, Physics* physics, sf::Texture* texture = nullptr);
	Wall(float x, float y, float radius, Physics* physics, sf::Texture* texture = nullptr);
	Wall(float x, float y, std::vector<sf::Vector2f>& vertices, Physics* physics, sf::Texture* texture = nullptr);
	~Wall();
};

