/*
* Written by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#include "pawn.hpp"

Pawn::Pawn(int id) : SceneNode(ReceiverCategories::kPlayer), m_player_id(id)
{
}

int Pawn::GetID() const
{
	return m_player_id;
}

bool Pawn::IsID(int id) const
{
	return m_player_id == id;
}