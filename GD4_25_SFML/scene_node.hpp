/*
* Written in class, edited by:
* Petr Sulc - GD4b - D00261476
* Jakub Polacek - GD4b - D00260171
*/

#pragma once
#include <SFML/Graphics.hpp>
#include "e_receiver_categories.hpp"
#include "command.hpp"
#include "command_queue.hpp"

class Collider;

class SceneNode : public sf::Transformable, public sf::Drawable
{
public:
	typedef std::unique_ptr<SceneNode> Ptr;

public:
	SceneNode(ReceiverCategories category = ReceiverCategories::kNone);
	void AttachChild(Ptr child);
	Ptr DetachChild(const SceneNode& node);

	void Update(sf::Time dt, CommandQueue& commands);

	sf::Vector2f GetWorldPosition() const;
	sf::Transform GetWorldTransform() const;

	void OnCommand(const Command& command, sf::Time dt);
	unsigned int GetCategory() const;
	void RemoveWrecks();
	void EvaluateCollision(Collider& other, CommandQueue& command_queue);
	SceneNode* GetParent();
	SceneNode* GetParent() const;

protected:
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands);
	virtual void OnCollision(Collider& other, CommandQueue& command_queue);

private:
	void UpdateChildren(sf::Time dt, CommandQueue& commands);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void DrawChildren(sf::RenderTarget& target, sf::RenderStates states) const;
	
	virtual bool IsMarkedForRemoval() const;
	virtual bool IsDestroyed() const;

private:
	std::vector<Ptr> m_children;
	SceneNode* m_parent;
	ReceiverCategories m_default_category;
};

