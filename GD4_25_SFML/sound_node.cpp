/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#include "sound_node.hpp"

SoundNode::SoundNode(SoundPlayer& player) : SceneNode(ReceiverCategories::kSoundNode), m_sounds(player)
{
}

void SoundNode::PlaySound(SoundID sound, sf::Vector2f position, float pitch)
{
    m_sounds.Play(sound, position, pitch);
}
