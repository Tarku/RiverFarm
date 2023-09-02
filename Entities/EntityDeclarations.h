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
	bool p_isHovered = false;

	static Texture* entitiesAtlas;
	static Texture* itemsAtlas;

public:
	Entity() {};
	virtual ~Entity()
	{
	};

	v2f position;
	v2f velocity;

	bool inWater = false;

	AtlasID atlasID = { 0, 0 };

	float speed = 4.f;

	virtual void HandleEvents(Event* event) abstract;
	virtual void Update(World* world, float dt) abstract;
	virtual void Draw(RenderWindow* window, v2f cameraPosition)
	{
		p_isHovered = CheckIfHovered(cameraPosition);
	}

	bool WillCollideWithBlock(const v2f& velocity, World* world);

	inline virtual bool CheckIfHovered(const v2f& cameraPosition)
	{
		FloatRect rect = GetRectangle();
		FloatRect screenRect = FloatRect(
			rect.left * (float)SCALED_TILE_SIZE - cameraPosition.x,
			rect.top * (float)SCALED_TILE_SIZE - cameraPosition.y,
			(float)SCALED_TILE_SIZE,
			(float)SCALED_TILE_SIZE
		);

		v2f mouseCoords = Utils::GetMousePosition();

		return screenRect.contains(mouseCoords);
	}

	inline virtual FloatRect GetRectangle()
	{
		return FloatRect(position.x, position.y, 1, 1);
	}

	inline static void DisposeEntityResources()
	{
		delete entitiesAtlas;
		delete itemsAtlas;
	}
};

class PlayerEntity : public Entity
{
private:
	bool m_didCollide = false;
	AtlasID m_directionalSpriteID = { 0, 0 };

public:
	PlayerEntity(const v2f& position);

	void HandleEvents(Event* event) override;
	void Update(World* world, float dt) override;
	void Draw(sf::RenderWindow* window, v2f cameraPosition) override;
};

class ItemEntity : public Entity
{
private:
	float m_scale = 1.f;

	float m_angle = 0.f;
	uchar m_itemID = -1;
	bool m_inPlayerRange = false;

	const float MAX_ITEM_LIFETIME = 5.f * 60.f;
	float m_lifetime = 0;

public:
	ItemEntity(const v2f& position, ItemID itemID);
	virtual ~ItemEntity() override;

	FloatRect GetRectangle() override;

	void HandleEvents(Event* event) override;
	void Update(World* world, float dt) override;
	void Draw(sf::RenderWindow* window, v2f cameraPosition) override;
};

class MovingEntity : public Entity
{
protected:
	const float MaxMoveTimer = 3.0f;

	float m_moveTimer = 0;

public:
	inline MovingEntity() {};

	inline void HandleEvents(Event* event) {};

	virtual void Update(World* world, float dt) override;
	virtual void RandomizeDirection() abstract;

	virtual void Draw(sf::RenderWindow* window, v2f cameraPosition) override;
};

class CowEntity : public MovingEntity
{
private:
	float m_hunger = 0;

	enum CowAIState
	{
		Wandering,
		LookingForFood,
		FoodTargetLocked
	};

	v2f m_foodTarget;

	CowAIState m_currentAIState = Wandering;

public:
	float milkFullness = 0;
	
	CowEntity(const v2f& position);

	void RandomizeDirection() override;

	void Update(World* world, float dt) override;
	void Draw(sf::RenderWindow* window, v2f cameraPosition) override;
};

#endif