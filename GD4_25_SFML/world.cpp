/*
* Written by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#include "world.hpp"
#include "level.hpp"
#include "sound_node.hpp"
#include "score.hpp"

World::World(sf::RenderTarget& output_target, FontHolder& font, SoundPlayer& sounds, ShaderHolder& shaders, GameData & game_data)
	: m_target(output_target)
	, m_camera(output_target.getDefaultView())
	, m_textures()
	, m_fonts(font)
	, m_sounds(sounds)
	, m_shaders(shaders)
	, m_scene_graph(ReceiverCategories::kScene)
	, m_world_bounds(sf::Vector2f(0.f, 0.f), sf::Vector2f(m_camera.getSize().x, m_camera.getSize().y))
	, m_physics()
	, m_game_data(game_data)
{
	static_cast<void>(m_scene_texture.resize({ m_target.getSize().x, m_target.getSize().y }));
	LoadTextures();
	BuildScene();
}

void World::Update(sf::Time dt)
{
	UpdateSounds();

	m_physics.SimulateAllBodies(dt);
	HandleCollisions();

	//Process commands from the scenegraph
	while (!m_command_queue.IsEmpty())
	{
		m_scene_graph.OnCommand(m_command_queue.Pop(), dt);
	}
	
	m_scene_graph.RemoveWrecks();
	m_scene_graph.Update(dt, m_command_queue);
}

void World::Draw()
{
	m_scene_texture.clear();
	m_scene_texture.setView(m_camera);
	m_scene_texture.draw(m_scene_graph);
	m_scene_texture.display();

	sf::Shader& shader = m_shaders.Get(ShaderID::kCRTShaderFrag);
	shader.setUniform("texture", m_scene_texture.getTexture());

	sf::Vector2f output_size = static_cast<sf::Vector2f>(m_target.getSize());
	sf::VertexArray vertices(sf::PrimitiveType::TriangleStrip, 4);
	vertices[0] = sf::Vertex({ 0.f, 0.f }, sf::Color::White, { 0.f, 1.f });
	vertices[1] = sf::Vertex({ output_size.x, 0 }, sf::Color::White, { 1, 1 });
	vertices[2] = sf::Vertex({ 0, output_size.y }, sf::Color::White, { 0, 0 });
	vertices[3] = sf::Vertex(sf::Vector2f(output_size), sf::Color::White, { 1, 0 });

	sf::RenderStates state;
	state.shader = &shader;
	state.blendMode = sf::BlendNone;

	m_target.draw(vertices, state);
}

CommandQueue& World::GetCommandQueue()
{
	return m_command_queue;
}

void World::LoadTextures()
{
	m_textures.Load(TextureID::kWallGrey, "Media/Textures/t_wall_grey.png");
	m_textures.Load(TextureID::kWallRed, "Media/Textures/t_wall_red.png");
	m_textures.Load(TextureID::kStoneWhite, "Media/Textures/t_flat_stone_white.png");
	m_textures.Load(TextureID::kStoneGrey, "Media/Textures/t_flat_stone_grey.png");
	m_textures.Load(TextureID::kStoneBlack, "Media/Textures/t_flat_stone_black.png");
	m_textures.Load(TextureID::kTileGrey, "Media/Textures/t_tile_grey.png");
	m_textures.Load(TextureID::kTileChess, "Media/Textures/t_tile_chess.png");
	m_textures.Load(TextureID::kTileGreen, "Media/Textures/t_tile_green.png");
	m_textures.Load(TextureID::kFire, "Media/Textures/t_paddle_fire.png");
	m_textures.Load(TextureID::kAquamarine, "Media/Textures/t_paddle_aquamarine.png");
	m_textures.Load(TextureID::kWater, "Media/Textures/t_paddle_water.png");
	m_textures.Get(TextureID::kWallGrey).setRepeated(true);
	m_textures.Get(TextureID::kWallRed).setRepeated(true);
	m_textures.Get(TextureID::kStoneWhite).setRepeated(true);
	m_textures.Get(TextureID::kStoneGrey).setRepeated(true);
	m_textures.Get(TextureID::kStoneBlack).setRepeated(true);
	m_textures.Get(TextureID::kTileGrey).setRepeated(true);
	m_textures.Get(TextureID::kTileChess).setRepeated(true);
	m_textures.Get(TextureID::kTileGreen).setRepeated(true);
	m_textures.Get(TextureID::kFire).setRepeated(true);
	m_textures.Get(TextureID::kAquamarine).setRepeated(true);
	m_textures.Get(TextureID::kWater).setRepeated(true);
}

void World::BuildScene()
{
	std::unique_ptr<SoundNode> soundNode(new SoundNode(m_sounds));
	m_scene_graph.AttachChild(std::move(soundNode));

	switch (m_game_data.GetSelectedLevel())
	{
	case 0:
		Level::CreateClassic(m_scene_graph, m_physics, m_textures, m_world_bounds, m_sounds, m_game_data, m_command_queue);
		break;
	case 1:
		Level::CreateDiamond(m_scene_graph, m_physics, m_textures, m_world_bounds, m_sounds, m_game_data, m_command_queue);
		break;
	default:
		break;
	}
	
	std::unique_ptr<Score> score(new Score(m_world_bounds.getCenter().x, m_world_bounds.getCenter().y - 250, m_fonts, m_game_data));
	m_scene_graph.AttachChild(std::move(score));
}

sf::FloatRect World::GetViewBounds() const
{
	return sf::FloatRect(m_camera.getCenter() - m_camera.getSize() / 2.f, m_camera.getSize());;
}

void World::HandleCollisions()
{
	std::vector<Physics::Pair> results;
	m_physics.EvaluateAllCollisions(results);

	for (auto& collision : results)
	{
		collision.first->EvaluateCollision(*collision.second, m_command_queue);
		collision.second->EvaluateCollision(*collision.first, m_command_queue);
	}
}

void World::UpdateSounds()
{
	sf::Vector2f listener_position;

	listener_position = m_camera.getCenter();

	m_sounds.SetListenerPosition(listener_position);

	m_sounds.RemoveStoppedSounds();
}