/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#pragma once
enum CollisionLayer
{
	kDefault = 1 << 0,
	kPlayer = 1 << 1,
	kBall = 1 << 2,
	kWall = 1 << 3,
	kPlayerBarrier = 1 << 4,
	kPickup = 1 << 5,
	kGoal = 1 << 6,
};