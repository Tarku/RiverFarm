#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include "Shared.h"
#include "ItemRegistry.h"
#include "AtlasManager.h"
#include <vector>

using namespace sf;

class World;

class Entity
{
public:
	Entity() {};

	v2f position;
	v2f velocity;

	AtlasID atlasID = { 0, 0 };
	Texture texture;

	virtual void HandleEvents(Event* event) = 0;
	virtual void Update(World* world, float dt) = 0;
	virtual void Draw(RenderWindow* window) = 0;
};

class PlayerEntity : public Entity, public std::enable_shared_from_this<PlayerEntity>
{
private:
	float m_speed = TILE_SIZE * TEXTURE_SCALE * 16;

public:
	PlayerEntity(const v2f& position);

	void HandleEvents(Event* event) override;
	void Update(World* world, float dt) override;
	void Draw(sf::RenderWindow* window) override;
};

class ItemEntity : public Entity
{
private:
	uchar m_itemID = -1;

public:
	ItemEntity(const v2f& position, ItemID itemID);

	void HandleEvents(Event* event) override;
	void Update(World* world, float dt) override;
	void Draw(sf::RenderWindow* window) override;
};

#endif