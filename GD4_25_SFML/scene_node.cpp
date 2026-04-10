/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#include "scene_node.hpp"
#include "utility.hpp"

SceneNode::SceneNode(ReceiverCategories category) : m_children(), m_parent(nullptr), m_default_category(category)
{
}

void SceneNode::AttachChild(Ptr child)
{
	child->m_parent = this;
	m_children.emplace_back(std::move(child));
}


SceneNode::Ptr SceneNode::DetachChild(const SceneNode& node)
{
	auto found = std::find_if(m_children.begin(), m_children.end(), [&](Ptr& p) {return p.get() == &node; });
	assert(found != m_children.end());
	
	Ptr result = std::move(*found);
	result->m_parent = nullptr;
	m_children.erase(found);
	return result;
}

void SceneNode::Update(sf::Time dt, CommandQueue& commands)
{
	UpdateCurrent(dt, commands);
	UpdateChildren(dt, commands);
}

sf::Vector2f SceneNode::GetWorldPosition() const
{
	return GetWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::GetWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;
	for (const SceneNode* node = this; node != nullptr; node = node->m_parent)
	{
		transform = node->getTransform() * transform;
	}
	return transform;
}

void SceneNode::OnCommand(const Command& command, sf::Time dt)
{
	//Is this command for me. If so execute it
	//Regardless of answer send to all children
	if (command.category & GetCategory())
	{
		command.action(*this, dt);
	}

	//Pass it on to children
	for (Ptr& child : m_children)
	{
		child->OnCommand(command, dt);
	}
}

void SceneNode::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
}

void SceneNode::OnCollision(Collider& other, CommandQueue& command_queue)
{
}

void SceneNode::UpdateChildren(sf::Time dt, CommandQueue& commands)
{
	for (Ptr& child : m_children)
	{
		child->Update(dt, commands);
	}
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//Apply the transform to the current node
	states.transform *= getTransform();
	//Draw the node and its children with the changed transform
	DrawCurrent(target, states);
	DrawChildren(target, states);
}

void SceneNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
}

void SceneNode::DrawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const Ptr& child : m_children)
	{
		child->draw(target, states);
	}
}

unsigned int SceneNode::GetCategory() const
{
	return static_cast<unsigned int>(m_default_category);
}

bool SceneNode::IsMarkedForRemoval() const
{
	return IsDestroyed();
}

bool SceneNode::IsDestroyed() const
{
	return false;
}

void SceneNode::RemoveWrecks()
{
	auto wreck_field_begin = std::remove_if(m_children.begin(), m_children.end(), std::mem_fn(&SceneNode::IsMarkedForRemoval));
	m_children.erase(wreck_field_begin, m_children.end());
	std::for_each(m_children.begin(), m_children.end(), std::mem_fn(&SceneNode::RemoveWrecks));
}

void SceneNode::EvaluateCollision(Collider& other, CommandQueue& command_queue)
{
	OnCollision(other, command_queue);
	if(m_parent != nullptr)
	{
		m_parent->OnCollision(other, command_queue);
		for (auto& child : m_parent->m_children)
		{
			child->OnCollision(other, command_queue);
		}
	}
}

SceneNode* SceneNode::GetParent()
{
	return m_parent;
}

SceneNode* SceneNode::GetParent() const
{
	return m_parent;
}
