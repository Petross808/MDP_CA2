/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <string>
#include <iostream>

#include "resource_holder.hpp"
#include "e_texture_id.hpp"
#include "e_font_id.hpp"
#include "e_shader_id.hpp"
#include "e_sound_id.hpp"


template<typename Identifier, typename Resource>
void ResourceHolder<Identifier, Resource>::Load(const Identifier id, const std::string& filename)
{
    bool loaded = false;
    std::unique_ptr<Resource> resource(new Resource());
    if constexpr (std::is_same_v<Resource, sf::Font>)
    {
        loaded = resource->openFromFile(filename);
    }
    else if constexpr (std::is_same_v<Resource, sf::Shader>)
    {
        loaded = resource->loadFromFile(filename, sf::Shader::Type::Fragment);
    }
    else
    {
        loaded = resource->loadFromFile(filename);
    }
    if(!loaded)
    {
        throw std::runtime_error("ResourceHolder::Load failed to load " + filename);
    }
    auto inserted = m_resource_map.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second); 
}

template<typename Identifier, typename Resource>
template<typename Parameter>
void ResourceHolder<Identifier, Resource>::Load(const Identifier id, const std::string& filename, const Parameter& second_param)
{
    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->loadFromFile(filename, second_param))
    {
        throw std::runtime_error("ResourceHolder::Load failed to load " + filename);
    }
    auto inserted = m_resource_map.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second);
}


template<typename Identifier, typename Resource>
const Resource& ResourceHolder<Identifier, Resource>::Get(Identifier id) const
{
    auto found = m_resource_map.find(id);
    assert(found != m_resource_map.end());
    return *found->second;
}


template<typename Identifier, typename Resource>
Resource& ResourceHolder<Identifier, Resource>::Get(Identifier id)
{
    auto found = m_resource_map.find(id);
    assert(found != m_resource_map.end());
    return *found->second;
}

template class ResourceHolder<TextureID, sf::Texture>;
template class ResourceHolder<FontID, sf::Font>;
template class ResourceHolder<ShaderID, sf::Shader>;
template class ResourceHolder<SoundID, sf::SoundBuffer>;
template void ResourceHolder<ShaderID, sf::Shader>::Load<sf::Shader::Type>(const ShaderID, const std::string&, const sf::Shader::Type&);