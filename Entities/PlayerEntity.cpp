#include "EntityDeclarations.h"
#include "../World/World.h"
#include "../World/TileRegistry.h"
#include "../Utils.h"

Texture* Entity::entitiesAtlas = AtlasManager::GetAtlas(AtlasTextureID::Entities);

PlayerEntity::PlayerEntity(const v2f& _position)
{
	position = _position;

	atlasID = { 0, 0 };

	World::WorldEntities.push_back(this);

	printf("Player was added.\n");
}

void PlayerEntity::HandleEvents(Event* event)
{
	switch (event->type)
	{
	case sf::Event::KeyPressed:
		if (event->key.code == Keyboard::Z)
			velocity.y = -1;

		if (event->key.code == Keyboard::S)
			velocity.y = 1;

		if (event->key.code == Keyboard::Q)
			velocity.x = -1;

		if (event->key.code == Keyboard::D)
			velocity.x = 1;
	}
}

void PlayerEntity::Update(World* world, float dt)
{
	bool doesCollide = false;

	for (int yOffset = -1; yOffset < 2; yOffset++)
	{
		for (int xOffset = -1; xOffset < 2; xOffset++)
		{
			if (position.x + xOffset < 0 || position.y + yOffset < 0 || position.x + xOffset >= MAP_WIDTH || position.y + yOffset >= MAP_HEIGHT)
				continue;

			m_inWater = (world->TileAt(position.x + xOffset, position.y + yOffset, 0) == TileID::Water);

			if (!(TileRegistry::Tiles[world->TileAt(position.x + xOffset, position.y + yOffset, 1)].tileProperties.isSolid))
				continue;

			if (FloatRect(position.x + 0.5f, position.y + 0.5f, .5f, .5f).intersects(FloatRect((position.x) + xOffset, (position.y) + yOffset, .5f, .5f)))
			{
				doesCollide = true;
			}
		}
	}

	m_speed = m_inWater ? 1.5f : 4.f;
	atlasID = m_inWater ? AtlasID(0, 1) : AtlasID(0, 0);

	if (doesCollide)
		position -= velocity * dt * m_speed;
	else
		position += velocity * dt * m_speed;
	velocity = v2f(0, 0);

	m_didCollide = doesCollide;
}

void PlayerEntity::Draw(sf::RenderWindow* window, v2f cameraPosition)
{
	Sprite s = Sprite(*Entity::entitiesAtlas, IntRect(atlasID.x * TILE_SIZE, atlasID.y * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	//s.setPosition(v2f(0, 0));
	s.setPosition(v2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
	s.setScale(TEXTURE_SCALE, TEXTURE_SCALE);

	window->draw(s);
}