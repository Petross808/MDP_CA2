/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#include "settings_state.hpp"
#include "state_stack.hpp"
#include "Utility.hpp"

SettingsState::SettingsState(StateStack& stack)
    : State(stack)
    , m_gui_container()
    , m_background_sprite(GetContext().textures->Get(TextureID::kTitleScreen))
{
    StateStack::Context context = GetContext();

    float xOffset = 450;
    for (auto& player : *context.players)
    {
        Action* actionArray = player.GetActionArray();

        float yOffset = 70;
        for (int x = 0; x < kActionCount; x++)
        {
            Action* a = &actionArray[x];
            m_buttonLabel_vector.emplace_back(ButtonLabel(a, 80 + xOffset, 150 + yOffset, context, m_gui_container));
            yOffset += 50;
        }
        m_buttonLabel_vector.shrink_to_fit();

        xOffset += 300;
    }

	auto back_button = std::make_shared<gui::Button>(GetContext());
    back_button->setPosition(sf::Vector2f(700.f, 600.f));
    back_button->SetText("Back");
    back_button->SetCallback(std::bind(&SettingsState::RequestStackPop, this));
    m_gui_container.Pack(back_button);
}

void SettingsState::Draw()
{
    sf::RenderWindow& window = *GetContext().window;
    window.draw(m_background_sprite);
    window.draw(m_gui_container);
}

bool SettingsState::Update(sf::Time dt)
{
    return true;
}

bool SettingsState::HandleEvent(const sf::Event& event)
{
    bool is_key_binding = false;

    for (ButtonLabel& buttonLabel : m_buttonLabel_vector)
    {
        if (buttonLabel.m_button->IsActive())
        {
            is_key_binding = true;
            const auto* key_released = event.getIf<sf::Event::KeyReleased>();
            if (key_released)
            {
                buttonLabel.m_action->ChangeKeybind(key_released->scancode);
                buttonLabel.m_button->Deactivate();
                buttonLabel.m_label->SetText(Utility::toString(key_released->scancode));
            }
            break;
        }
    }

    if (!is_key_binding)
    {
        m_gui_container.HandleEvent(event);
    }
    return false;
}

SettingsState::ButtonLabel::ButtonLabel(Action* action, float x, float y, StateStack::Context& context, gui::Container& container) :
    m_action(action),
    m_button(std::make_shared<gui::Button>(context)),
    m_label(std::make_shared<gui::Label>("", *context.fonts))
{
    m_button->setPosition(sf::Vector2f(x, y));
    m_button->SetText(action->GetName());
    m_button->SetToggle(true);

    m_label->setPosition(sf::Vector2f(220.f + x, y + 15.f));
    m_label->SetText(Utility::toString(action->GetKeyBind()));

    container.Pack(m_button);
    container.Pack(m_label);
}
