/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#pragma once
enum CollisionLayer
{
	kDefault = 1 << 0,
	kPlayer = 1 << 1,
	kPlayerDisabled = 1 << 2,
	kBall = 1 << 3,
	kWall = 1 << 4,
	kPlayerBarrier = 1 << 5,
	kPickup = 1 << 6,
	kGoal = 1 << 7,
};