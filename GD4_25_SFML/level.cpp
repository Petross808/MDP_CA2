/*
* Written by:
* Jakub Polacek - GD4b - D00260171
*/

#include "level.hpp"
#include "player_spawn.hpp"
#include "wall.hpp"
#include "ball.hpp"
#include "pickup_spawner.hpp"
#include "shape_node.hpp"
#include "player_barrier.hpp"
#include "goal.hpp"

void Level::CreateBounds(SceneNode& root, Physics& physics, sf::FloatRect world_bounds, float wall_width, sf::Texture* texture)
{

	std::unique_ptr<Wall> wall1(new Wall(0, 0, world_bounds.size.x, wall_width, &physics, texture));
	std::unique_ptr<Wall> wall2(new Wall(0, world_bounds.size.y - wall_width, world_bounds.size.x, wall_width, &physics, texture));
	std::unique_ptr<Wall> wall3(new Wall(0, 0, wall_width, world_bounds.size.y, &physics, texture));
	std::unique_ptr<Wall> wall4(new Wall(world_bounds.size.x - wall_width, 0, wall_width, world_bounds.size.y, &physics, texture));

	root.AttachChild(std::move(wall1));
	root.AttachChild(std::move(wall2));
	root.AttachChild(std::move(wall3));
	root.AttachChild(std::move(wall4));
}


void Level::CreateClassic(SceneNode& root, Physics& physics, sf::FloatRect world_bounds, std::default_random_engine& rand, GameData& data, CommandQueue& command_queue, TextureHolder* texture_holder, SoundPlayer* sounds)
{
	sf::Texture* wallGrey = texture_holder ? &texture_holder->Get(TextureID::kWallGrey) : nullptr;
	sf::Texture* wallRed = texture_holder ? &texture_holder->Get(TextureID::kWallRed) : nullptr;
	sf::Texture* stoneWhite = texture_holder ? &texture_holder->Get(TextureID::kStoneWhite) : nullptr;
	sf::Texture* stoneBlack = texture_holder ? &texture_holder->Get(TextureID::kStoneBlack) : nullptr;
	sf::Texture* tileGrey = texture_holder ? &texture_holder->Get(TextureID::kTileGrey) : nullptr;
	sf::Texture* tileChess = texture_holder ? &texture_holder->Get(TextureID::kTileChess) : nullptr;
	sf::Texture* tileGreen = texture_holder ? &texture_holder->Get(TextureID::kTileGreen) : nullptr;
	sf::Texture* fire = texture_holder ? &texture_holder->Get(TextureID::kFire) : nullptr;


	std::unique_ptr<SceneNode> background(new SceneNode());
	std::unique_ptr<SceneNode> walls(new SceneNode());
	std::unique_ptr<SceneNode> dynamic(new SceneNode());

	CreateBounds(*walls, physics, world_bounds, 10, wallRed);

	sf::Vector2f center(world_bounds.getCenter());

	std::unique_ptr<Ball> ball(new Ball(center.x - 20, center.y - 20, 20, &physics, rand, fire));
	dynamic->AttachChild(std::move(ball));

	
	std::unique_ptr<PlayerSpawn> team_one(new PlayerSpawn(0, data.IsOnline(), 200, center.y, physics, command_queue, sounds, stoneWhite));
	dynamic->AttachChild(std::move(team_one));

	std::unique_ptr<PlayerSpawn> team_two(new PlayerSpawn(1, data.IsOnline(), world_bounds.size.x - 200, center.y, physics, command_queue, sounds, stoneWhite));
	dynamic->AttachChild(std::move(team_two));
	

	if (texture_holder)
	{
		std::unique_ptr<PickupSpawner> pickupSpawner(new PickupSpawner(450, 20, 640, 800, &physics, rand, *sounds, *texture_holder, 15));
		dynamic->AttachChild(std::move(pickupSpawner));

		std::unique_ptr<ShapeNode> player_one_zone(new ShapeNode(400, world_bounds.size.y));
		player_one_zone->setPosition({ 0,0 });
		player_one_zone->SetTexture(*tileGrey);
		background->AttachChild(std::move(player_one_zone));

		std::unique_ptr<ShapeNode> player_two_zone(new ShapeNode(400, world_bounds.size.y));
		player_two_zone->setPosition({ world_bounds.size.x - 400, 0 });
		player_two_zone->SetTexture(*tileGrey);
		background->AttachChild(std::move(player_two_zone));

		std::unique_ptr<ShapeNode> playing_field(new ShapeNode(800, world_bounds.size.y));
		playing_field->setPosition({ 400, 0 });
		playing_field->SetTexture(*tileGreen);
		background->AttachChild(std::move(playing_field));
	}

	std::unique_ptr<PlayerBarrier> player_one_barrier(new PlayerBarrier(400, 0, 50, world_bounds.size.y, &physics));
	background->AttachChild(std::move(player_one_barrier));

	std::unique_ptr<PlayerBarrier> player_two_barrier(new PlayerBarrier(world_bounds.size.x - 450, 0, 50, world_bounds.size.y, &physics));
	background->AttachChild(std::move(player_two_barrier));

	std::unique_ptr<Goal> player_one_goal(new Goal(0, 0, 0, 50, world_bounds.size.y, &physics, tileChess));
	background->AttachChild(std::move(player_one_goal));

	std::unique_ptr<Goal> player_two_goal(new Goal(1, world_bounds.size.x - 50, 0, 50, world_bounds.size.y, &physics, tileChess));
	background->AttachChild(std::move(player_two_goal));

	root.AttachChild(std::move(background));
	root.AttachChild(std::move(walls));
	root.AttachChild(std::move(dynamic));
}

void Level::CreateJagged(SceneNode& root, Physics& physics, sf::FloatRect world_bounds, std::default_random_engine& rand, GameData& data, CommandQueue& command_queue, TextureHolder* texture_holder, SoundPlayer* sounds)
{
	sf::Texture* wallGrey = texture_holder ? &texture_holder->Get(TextureID::kWallGrey) : nullptr;
	sf::Texture* wallRed = texture_holder ? &texture_holder->Get(TextureID::kWallRed) : nullptr;
	sf::Texture* stoneWhite = texture_holder ? &texture_holder->Get(TextureID::kStoneWhite) : nullptr;
	sf::Texture* stoneBlack = texture_holder ? &texture_holder->Get(TextureID::kStoneBlack) : nullptr;
	sf::Texture* tileGrey = texture_holder ? &texture_holder->Get(TextureID::kTileGrey) : nullptr;
	sf::Texture* tileChess = texture_holder ? &texture_holder->Get(TextureID::kTileChess) : nullptr;
	sf::Texture* tileGreen = texture_holder ? &texture_holder->Get(TextureID::kTileGreen) : nullptr;
	sf::Texture* fire = texture_holder ? &texture_holder->Get(TextureID::kFire) : nullptr;

	std::unique_ptr<SceneNode> background(new SceneNode());
	std::unique_ptr<SceneNode> walls(new SceneNode());
	std::unique_ptr<SceneNode> dynamic(new SceneNode());

	CreateBounds(*walls, physics, world_bounds, 10, wallRed);

	sf::Vector2f center(world_bounds.getCenter());

	std::unique_ptr<Ball> ball(new Ball(center.x - 20, center.y - 20, 20, &physics, rand, fire));
	dynamic->AttachChild(std::move(ball));

	std::unique_ptr<PlayerSpawn> team_one(new PlayerSpawn(0, data.IsOnline(), 200, center.y, physics, command_queue, sounds, stoneWhite));
	dynamic->AttachChild(std::move(team_one));

	std::unique_ptr<PlayerSpawn> team_two(new PlayerSpawn(1, data.IsOnline(), world_bounds.size.x - 200, center.y, physics, command_queue, sounds, stoneWhite));
	dynamic->AttachChild(std::move(team_two));


	if (texture_holder)
	{
		std::unique_ptr<PickupSpawner> pickupSpawner(new PickupSpawner(450, 20, 640, 800, &physics, rand, *sounds, *texture_holder, 15));
		dynamic->AttachChild(std::move(pickupSpawner));

		std::unique_ptr<ShapeNode> player_one_zone(new ShapeNode(400, world_bounds.size.y));
		player_one_zone->setPosition({ 0,0 });
		player_one_zone->SetTexture(*tileGrey);
		background->AttachChild(std::move(player_one_zone));

		std::unique_ptr<ShapeNode> player_two_zone(new ShapeNode(400, world_bounds.size.y));
		player_two_zone->setPosition({ world_bounds.size.x - 400, 0 });
		player_two_zone->SetTexture(*tileGrey);
		background->AttachChild(std::move(player_two_zone));

		std::unique_ptr<ShapeNode> playing_field(new ShapeNode(800, world_bounds.size.y));
		playing_field->setPosition({ 400, 0 });
		playing_field->SetTexture(*tileGreen);
		background->AttachChild(std::move(playing_field));
	}

	std::unique_ptr<PlayerBarrier> player_one_barrier(new PlayerBarrier(400, 0, 50, world_bounds.size.y, &physics));
	background->AttachChild(std::move(player_one_barrier));

	std::unique_ptr<PlayerBarrier> player_two_barrier(new PlayerBarrier(world_bounds.size.x - 450, 0, 50, world_bounds.size.y, &physics));
	background->AttachChild(std::move(player_two_barrier));

	std::unique_ptr<Goal> player_one_goal(new Goal(0, 0, 0, 50, world_bounds.size.y, &physics, tileChess));
	background->AttachChild(std::move(player_one_goal));

	std::unique_ptr<Goal> player_two_goal(new Goal(1, world_bounds.size.x - 50, 0, 50, world_bounds.size.y, &physics, tileChess));
	background->AttachChild(std::move(player_two_goal));

	
	std::vector<sf::Vector2f> diamondShape = { {-150, 0}, {0,50}, {100, 0}, {0,-50} };
	std::vector<sf::Vector2f> diamondShape2 = { {-100, 0}, {0,50}, {150, 0}, {0,-50} };

	std::unique_ptr<Wall> obstacle(new Wall(center.x - 200, center.y, diamondShape, &physics, wallGrey));
	walls->AttachChild(std::move(obstacle));

	std::unique_ptr<Wall> obstacle2(new Wall(center.x + 200, center.y, diamondShape2, &physics, wallGrey));
	walls->AttachChild(std::move(obstacle2));
	
	std::vector<sf::Vector2f> bigDiamondShape = { {-500, 0},{0,150},{500, 0},{0,-150} };

	std::unique_ptr<Wall> obstacle3(new Wall(center.x, 0, bigDiamondShape, &physics, wallGrey));
	walls->AttachChild(std::move(obstacle3));

	std::unique_ptr<Wall> obstacle4(new Wall(center.x, world_bounds.size.y, bigDiamondShape, &physics, wallGrey));
	walls->AttachChild(std::move(obstacle4));

	root.AttachChild(std::move(background));
	root.AttachChild(std::move(walls));
	root.AttachChild(std::move(dynamic));
}

void Level::CreateDeadly(SceneNode& root, Physics& physics, sf::FloatRect world_bounds, std::default_random_engine& rand, GameData& data, CommandQueue& command_queue, TextureHolder* texture_holder, SoundPlayer* sounds)
{
	sf::Texture* wallGrey = texture_holder ? &texture_holder->Get(TextureID::kWallGrey) : nullptr;
	sf::Texture* wallRed = texture_holder ? &texture_holder->Get(TextureID::kWallRed) : nullptr;
	sf::Texture* stoneWhite = texture_holder ? &texture_holder->Get(TextureID::kStoneWhite) : nullptr;
	sf::Texture* stoneBlack = texture_holder ? &texture_holder->Get(TextureID::kStoneBlack) : nullptr;
	sf::Texture* tileGrey = texture_holder ? &texture_holder->Get(TextureID::kTileGrey) : nullptr;
	sf::Texture* tileChess = texture_holder ? &texture_holder->Get(TextureID::kTileChess) : nullptr;
	sf::Texture* tileGreen = texture_holder ? &texture_holder->Get(TextureID::kTileGreen) : nullptr;
	sf::Texture* fire = texture_holder ? &texture_holder->Get(TextureID::kFire) : nullptr;

	std::unique_ptr<SceneNode> background(new SceneNode());
	std::unique_ptr<SceneNode> walls(new SceneNode());
	std::unique_ptr<SceneNode> dynamic(new SceneNode());

	sf::Vector2f center(world_bounds.getCenter());

	CreateBounds(*walls, physics, world_bounds, 10, wallRed);
	
	std::unique_ptr<Ball> ball(new Ball(center.x - 20, center.y - 20, 20, &physics, rand, fire));
	dynamic->AttachChild(std::move(ball));

	std::unique_ptr<PlayerSpawn> team_one(new PlayerSpawn(0, data.IsOnline(), 200, center.y, physics, command_queue, sounds, stoneWhite));
	dynamic->AttachChild(std::move(team_one));

	std::unique_ptr<PlayerSpawn> team_two(new PlayerSpawn(1, data.IsOnline(), world_bounds.size.x - 200, center.y, physics, command_queue, sounds, stoneWhite));
	dynamic->AttachChild(std::move(team_two));

	if (texture_holder)
	{
		std::unique_ptr<PickupSpawner> pickupSpawner(new PickupSpawner(600, 20, 340, 800, &physics, rand, *sounds, *texture_holder, 15));
		dynamic->AttachChild(std::move(pickupSpawner));

		std::unique_ptr<ShapeNode> team_one_zone(new ShapeNode(600, world_bounds.size.y));
		team_one_zone->setPosition({ 0,0 });
		team_one_zone->SetTexture(*tileGrey);
		background->AttachChild(std::move(team_one_zone));

		std::unique_ptr<ShapeNode> team_two_zone(new ShapeNode(600, world_bounds.size.y));
		team_two_zone->setPosition({ 1000, 0 });
		team_two_zone->SetTexture(*tileGrey);
		background->AttachChild(std::move(team_two_zone));

		std::unique_ptr<ShapeNode> playing_field(new ShapeNode(400, world_bounds.size.y));
		playing_field->setPosition({ 600, 0 });
		playing_field->SetTexture(*tileGreen);
		background->AttachChild(std::move(playing_field));
	}

	std::unique_ptr<PlayerBarrier> team_one_barrier(new PlayerBarrier(600, 0, 50, world_bounds.size.y, &physics));
	background->AttachChild(std::move(team_one_barrier));
	std::unique_ptr<PlayerBarrier> team_two_barrier(new PlayerBarrier(world_bounds.size.x - 650, 0, 50, world_bounds.size.y, &physics));
	background->AttachChild(std::move(team_two_barrier));

	std::unique_ptr<Goal> team_one_goal_middle(new Goal(0, 0, 0, 30, world_bounds.size.y, &physics, tileChess));
	std::unique_ptr<Goal> team_one_goal_top(new Goal(0, 30, 0, center.x - 230, 30, &physics, tileChess));
	std::unique_ptr<Goal> team_one_goal_bottom(new Goal(0, 30, world_bounds.size.y - 30, center.x - 230, 30, &physics, tileChess));
	background->AttachChild(std::move(team_one_goal_top));
	background->AttachChild(std::move(team_one_goal_middle));
	background->AttachChild(std::move(team_one_goal_bottom));

	std::unique_ptr<Goal> team_two_goal(new Goal(1, world_bounds.size.x - 30, 0, 30, world_bounds.size.y, &physics, tileChess));
	std::unique_ptr<Goal> team_two_goal_top(new Goal(1, center.x + 200, 0, center.x - 230, 30, &physics, tileChess));
	std::unique_ptr<Goal> team_two_goal_bottom(new Goal(1, center.x + 200, world_bounds.size.y - 30, center.x - 230, 30, &physics, tileChess));
	background->AttachChild(std::move(team_two_goal));
	background->AttachChild(std::move(team_two_goal_top));
	background->AttachChild(std::move(team_two_goal_bottom));

	root.AttachChild(std::move(background));
	root.AttachChild(std::move(walls));
	root.AttachChild(std::move(dynamic));
}