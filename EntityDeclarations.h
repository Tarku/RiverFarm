#pragma once

#include "Shared.h"
#include "ItemRegistry.h"
#include "AtlasManager.h"
#include <vector>

using namespace sf;

class Entity
{
public:
	Entity() {};

	static std::vector<Entity*> Entities;

	v2f position;
	v2f velocity;

	AtlasID atlasID = { 0, 0 };
	Texture texture;

	virtual void HandleEvents(Event* event) = 0;
	virtual void Update(float dt) = 0;
	virtual void Draw(RenderWindow* window) = 0;
};

class PlayerEntity : public Entity
{
private:
	float m_speed = TILE_SIZE * TEXTURE_SCALE * 4;

public:
	PlayerEntity(const v2f& position);

	void HandleEvents(Event* event) override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow* window) override;
};

class ItemEntity : public Entity
{

public:
	ItemEntity(const v2f& position, ItemID itemID);

	virtual void HandleEvents(Event* event) = 0;
	void Update(float dt) override;
	void Draw(sf::RenderWindow* window) override;
};