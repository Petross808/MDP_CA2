/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#pragma once
#include <SFML/Graphics/Font.hpp>

#include "resource_holder.hpp"
#include "e_texture_id.hpp"
#include "e_font_id.hpp"
#include "e_shader_id.hpp"
#include "e_sound_id.hpp"

namespace sf
{
	class Texture;
	class Font;
	class Shader;
	class SoundBuffer;
}

//template<typename Identifier, typename Resource>

typedef ResourceHolder<TextureID, sf::Texture> TextureHolder;
typedef ResourceHolder<FontID, sf::Font> FontHolder;
typedef ResourceHolder<ShaderID, sf::Shader> ShaderHolder;
typedef ResourceHolder<SoundID, sf::SoundBuffer> SoundBufferHolder;