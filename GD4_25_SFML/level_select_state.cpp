/*
* Written by:
* Petr Sulc - GD4b - D00261476
*/

#include "level_select_state.hpp"
#include "button.hpp"
#include "game_state.hpp"
#include "label.hpp"
#include "utility.hpp"

LevelSelectState::LevelSelectState(StateStack& stack) :
	State(stack),
	m_background_sprite(GetContext().textures->Get(TextureID::kTitleScreen)),
	m_gui_container()
{
	auto data = GetContext().game_data;
	float x = 700;
	float y = 350;
	float xLabel = x + 220;
	float yLabel = y + 15;

	gui::Label::Ptr levelLabel = std::make_shared<gui::Label>("", *GetContext().fonts);
	levelLabel->setPosition(sf::Vector2f(xLabel, yLabel));
	levelLabel->SetText(data->GetLevelName());
	auto level = std::make_shared<gui::Button>(GetContext());
	level->setPosition(sf::Vector2f(x, y));
	level->SetText("Level");
	level->SetCallback([this, levelLabel, data]()
		{
			data->CycleLevel();
			levelLabel->SetText(data->GetLevelName());
		});

	m_gui_container.Pack(levelLabel);
	m_gui_container.Pack(level);

	for (int i = 0; i < kLocalPlayerCount; ++i)
	{
		gui::Label::Ptr playerLabel = std::make_shared<gui::Label>("", *GetContext().fonts);
		playerLabel->setPosition(sf::Vector2f(xLabel, 50.f + yLabel + 50.f * i));
		playerLabel->SetText(data->GetCharacterName(i));
		
		auto player_one = std::make_shared<gui::Button>(GetContext());
		player_one->setPosition(sf::Vector2f(x, 50.f + y + 50.f * i));
		player_one->SetText("Player " + std::to_string(i + 1));
		player_one->SetCallback([this, playerLabel, data, i]()
			{
				data->CycleCharacter(i);
				playerLabel->SetText(data->GetCharacterName(i));
			});
		m_gui_container.Pack(player_one);
		m_gui_container.Pack(playerLabel);
	}

	auto play_button = std::make_shared<gui::Button>(GetContext());
	play_button->setPosition(sf::Vector2f(x, y + 100 + kLocalPlayerCount*50));
	play_button->SetText("Play");
	play_button->SetCallback([this]()
		{
			RequestStackClear();
			RequestStackPush<GameState>();
		});
	m_gui_container.Pack(play_button);

	auto back_button = std::make_shared<gui::Button>(GetContext());
	back_button->setPosition(sf::Vector2f(x, y + 150 + kLocalPlayerCount * 50));
	back_button->SetText("Back");
	back_button->SetCallback(std::bind(&LevelSelectState::RequestStackPop, this));
	m_gui_container.Pack(back_button);
}

LevelSelectState::~LevelSelectState() = default;

void LevelSelectState::Draw()
{
	sf::RenderWindow& window = *GetContext().window;
	window.draw(m_background_sprite);
	window.draw(m_gui_container);
}

bool LevelSelectState::Update(sf::Time dt)
{
	return false;
}

bool LevelSelectState::HandleEvent(const sf::Event& event)
{
	m_gui_container.HandleEvent(event);
	return false;
}