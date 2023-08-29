#include "EntityDeclarations.h"
#include "../World/World.h"
#include "../World/Tiles/TileRegistry.h"
#include "../Utils.h"

Texture* Entity::entitiesAtlas = AtlasManager::GetAtlas(AtlasTextureID::Entities);

PlayerEntity::PlayerEntity(const v2f& _position)
{
	position = _position;

	atlasID = { 0, 0 };


	printf("Player was added.\n");
}

void PlayerEntity::HandleEvents(Event* event)
{

}

bool Entity::WillCollideWithBlock(const v2f& velocity, World* world)
{
	const float twoSixteenths = 2.f / 16.f;
	const float fourteenSixteenths = 14.f / 16.f;

	return
		TileRegistry::Tiles[world->TileAt(position.x + velocity.x + twoSixteenths, position.y + velocity.y + twoSixteenths, 1)]->tileProperties.isSolid
		|| TileRegistry::Tiles[world->TileAt(position.x + velocity.x + fourteenSixteenths, position.y + velocity.y + twoSixteenths, 1)]->tileProperties.isSolid
		|| TileRegistry::Tiles[world->TileAt(position.x + velocity.x + twoSixteenths, position.y + velocity.y + fourteenSixteenths, 1)]->tileProperties.isSolid
		|| TileRegistry::Tiles[world->TileAt(position.x + velocity.x + fourteenSixteenths, position.y + velocity.y + fourteenSixteenths, 1)]->tileProperties.isSolid;
}


void PlayerEntity::Update(World* world, float dt)
{
	if (sf::Keyboard::isKeyPressed(Keyboard::Z))
	{
		m_directionalSpriteID = { 0, 1 };

		if (WillCollideWithBlock(dt * v2f(0, -1) * 2.f, world))
		{
			velocity.y *= 0.1f;
		}
		else
		{
			velocity.y--;
		}
	}

	if (sf::Keyboard::isKeyPressed(Keyboard::S))
	{
		m_directionalSpriteID = { 0, 0 };

		if (WillCollideWithBlock(dt * v2f(0, 1) * 4.f, world))
		{
			velocity.y *= 0.1f;
		}
		else
		{
			velocity.y++;
		}
	}

	if (sf::Keyboard::isKeyPressed(Keyboard::Q))
	{
		m_directionalSpriteID = { 0, 2 };

		if (WillCollideWithBlock(dt * v2f(-1, 0) * 4.f, world))
		{
			velocity.x *= 0.1f;
		}
		else
		{
			velocity.x--;
		}
	}

	if (sf::Keyboard::isKeyPressed(Keyboard::D))
	{
		m_directionalSpriteID = { 0, 3 };
		if (WillCollideWithBlock(dt * v2f(1, 0) * 4.f, world))
		{
			velocity.x *= 0.1f;
		}
		else
		{
			velocity.x++;
		}
	}

	inWater = (world->TileAt(position.x + .5f, position.y + .5f, 0) == TileID::Water);

	speed = inWater ? 1.5f : 4.f;
	atlasID = inWater ? AtlasID(1, 1) : m_directionalSpriteID;

	position += velocity * dt * speed;

	velocity *= 0.25f;
}

void PlayerEntity::Draw(sf::RenderWindow* window, v2f cameraPosition)
{
	Sprite s = Sprite(*Entity::entitiesAtlas, IntRect(atlasID.x * TILE_SIZE, atlasID.y * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	s.setPosition(v2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
	s.setScale(TEXTURE_SCALE, TEXTURE_SCALE);

	window->draw(s);
}