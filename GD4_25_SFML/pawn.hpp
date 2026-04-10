/*
* Written by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/
#pragma once
#include "scene_node.hpp"

class Pawn : public SceneNode
{
public:
	Pawn(int id);
	int GetID() const;
	bool IsID(int id) const;
	
	virtual void ApplyMove(float x, float y) = 0;
	virtual void UsePickup() = 0;

private:
	int m_player_id;
};