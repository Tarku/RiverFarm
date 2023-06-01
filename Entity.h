#pragma once

#include "Shared.h"
#include "ItemRegistry.h"
#include <vector>

using namespace sf;

class Entity
{
public:
	Entity() {};

	static std::vector<Entity*> Entities;

	v2f position;
	v2f velocity;

	AtlasID atlasID;

	virtual void Update(float dt) = 0;
	virtual void Draw(RenderWindow* window) = 0;
};

class PlayerEntity : public Entity
{
public:
	PlayerEntity(const v2f& position);

	void Update(float dt) override;
	void Draw(sf::RenderWindow* window) override;
};

class ItemEntity : public Entity
{

public:
	ItemEntity(const v2f& position, ItemID itemID);

	void Update(float dt) override;
	void Draw(sf::RenderWindow* window) override;
};