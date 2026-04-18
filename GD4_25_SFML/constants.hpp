/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#pragma once
// Metadata
constexpr auto kWindowTitle = "Ultra Pong";
constexpr auto kWindowWidth = 1600;
constexpr auto kWindowHeight = 900;

constexpr auto kLocalPlayerCount = 2;
constexpr auto kActionCount = 5;

constexpr auto kTimePerFrame = 1.f / 60.f;

constexpr auto kInitialBallSpeed = 10.f;
constexpr auto kPickupDropChance = 3;
constexpr auto kPointsToWin = 3;
constexpr auto kGameOverToMenuPause = 2;

constexpr auto kLevelCount = 3;
constexpr auto kCharacterCount = 4;

constexpr auto kLogBandwidth = false;