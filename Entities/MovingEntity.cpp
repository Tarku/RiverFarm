#include "EntityDeclarations.h"
#include "../World/Tiles/TileRegistry.h"

void MovingEntity::Update(World* world, float dt)
{
	inWater = (world->TileAt(position.x + .5f, position.y + .5f, 0) == TileID::Water);

	speed = inWater ? 1.5f : speed;

	if (WillCollideWithBlock(velocity * dt, world))
	{
		float wallHitAngle = PI; // 3.14 radians = 90° degrees
		float s = sinf(wallHitAngle);
		float c = cosf(wallHitAngle);

		velocity.x = c * velocity.x - s * velocity.y;
		velocity.y = s * velocity.x + c * velocity.y;


	}
	else
		position += velocity * dt * speed;

	m_moveTimer += dt;

	if (m_moveTimer > MaxMoveTimer)
	{
		m_moveTimer = 0;
		OnMoveTimer();
	}
};

void MovingEntity::Draw(sf::RenderWindow* window, v2f cameraPosition)
{
	sf::Sprite spr = sf::Sprite(*entitiesAtlas);

	spr.setTextureRect(sf::IntRect(atlasID.x * TILE_SIZE, atlasID.y * TILE_SIZE, TILE_SIZE, TILE_SIZE));
	spr.setPosition(position * (float)SCALED_TILE_SIZE - cameraPosition);

	spr.setScale(TEXTURE_SCALE, TEXTURE_SCALE);

	window->draw(spr);


};