#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include "../Shared.h"
#include "../Inventory/ItemRegistry.h"
#include "../AtlasManager.h"

using namespace sf;

class World;

class Entity
{
protected:
	static Texture* entitiesAtlas;
	static Texture* itemsAtlas;

public:
	Entity() {};
	virtual ~Entity()
	{
	};

	v2f position;
	v2f velocity;

	AtlasID atlasID = { 0, 0 };

	virtual void HandleEvents(Event* event) abstract;
	virtual void Update(World* world, float dt) abstract;
	virtual void Draw(RenderWindow* window, v2f cameraPosition) abstract;


	inline virtual FloatRect GetRectangle()
	{
		return FloatRect(position.x, position.y, 1, 1);
	}

	inline static void DisposeEntityResources()
	{
		delete entitiesAtlas;
	}
};

class PlayerEntity : public Entity
{
private:
	float m_speed = 16;
	bool m_didCollide = false;
	bool m_inWater = false;

public:
	PlayerEntity(const v2f& position);

	void HandleEvents(Event* event) override;
	void Update(World* world, float dt) override;
	void Draw(sf::RenderWindow* window, v2f cameraPosition) override;
};

class ItemEntity : public Entity
{
private:
	uchar m_itemID = -1;
	bool m_inPlayerRange = false;

	const float MAX_ITEM_LIFETIME = 5.f * 60.f;
	float m_lifetime = 0;

public:
	ItemEntity(const v2f& position, ItemID itemID);
	virtual ~ItemEntity() override;

	void HandleEvents(Event* event) override;
	void Update(World* world, float dt) override;
	void Draw(sf::RenderWindow* window, v2f cameraPosition) override;
};

#endif