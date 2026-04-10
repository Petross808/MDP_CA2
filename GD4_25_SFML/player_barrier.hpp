/*
* Written by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#pragma once
#include "scene_node.hpp"
#include "physics.hpp"

class PlayerBarrier : public SceneNode
{
public:
	PlayerBarrier(float x, float y, float width, float height, Physics* physics);
	~PlayerBarrier();
};

