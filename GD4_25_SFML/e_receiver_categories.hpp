/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#pragma once
enum class ReceiverCategories
{
	kNone = 0,
	kScene = 1 << 0,
	kPlayer = 1 << 1,
	kBall = 1 << 2,
	kSoundNode = 1 << 3,
	kScore = 1 << 4
};
