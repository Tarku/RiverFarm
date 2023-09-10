#include "EntityDeclarations.h"
#include "../World/Tiles/TileRegistry.h"

void MovingEntity::Update(World* world, float dt)
{
	inWater = (world->TileAt(position.x + .5f, position.y + .5f, 0) == TileID::Water);

	float actualSpeed = inWater ? speed / 2 : speed;

	if (WillCollideWithBlock(velocity * dt, world))
	{
		RandomizeDirection();
	}
	else
	{
		position += velocity * dt * actualSpeed;
	}

	m_moveTimer += dt;

	if (m_moveTimer > MaxMoveTimer)
	{
		m_moveTimer = 0;
		RandomizeDirection();
	}
};

void MovingEntity::Draw(sf::RenderWindow* window, v2f cameraPosition)
{
	Entity::Draw(window, cameraPosition);

	sf::Sprite spr = sf::Sprite(*entitiesAtlas);

	spr.setTextureRect(sf::IntRect(v2i(atlasID.x * TILE_SIZE, atlasID.y * TILE_SIZE), v2i(TILE_SIZE, TILE_SIZE)));
	spr.setPosition(position * (float)SCALED_TILE_SIZE - cameraPosition);

	spr.setScale(v2f(TEXTURE_SCALE, TEXTURE_SCALE));

	window->draw(spr);


};