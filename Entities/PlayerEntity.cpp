#include "EntityDeclarations.h"
#include "../World/World.h"
#include "../World/Tiles/TileRegistry.h"
#include "../Utils.h"
#include "../OptionsManager.h"

#include "../World/Tiles/TileRegistry.h"

Texture* Entity::entitiesAtlas = AtlasManager::GetAtlas(AtlasTextureID::Entities);

std::map<TileID, std::map<Direction, AtlasID>> PlayerEntity::ContextualDirectionalSprites =
{
	{
			TileID::Water,
			std::map<Direction, AtlasID> {
				{
					Direction::North, AtlasID(2, 1)
				},
				{
					Direction::South, AtlasID(2, 0)
				},
				{
					Direction::West, AtlasID(2, 2)
				},
				{
					Direction::East, AtlasID(2, 3)
				}
			}
	},
		{
			TileID::Ice,
			std::map<Direction, AtlasID> {
				{
					Direction::North, AtlasID(3, 1)
				},
				{
					Direction::South, AtlasID(3, 0)
				},
				{
					Direction::West, AtlasID(3, 2)
				},
				{
					Direction::East, AtlasID(3, 3)
				}
			}
		}
};

std::map<Direction, AtlasID> PlayerEntity::DefaultDirectionalSprites =
		{
			{ Direction::North, AtlasID(0, 1) },
			{ Direction::South, AtlasID(0, 0) },
			{ Direction::West, AtlasID(0, 2) },
			{ Direction::East, AtlasID(0, 3) }
		};

PlayerEntity::PlayerEntity(const v2f& _position)
{
	position = _position;

	atlasID = { 0, 0 };

	maxHp = 1000;
	hp = maxHp;

	maxEnergy = 1000;
	energy = maxEnergy;

	Utils::Log(std::format("Player {} was added.\n", name));
}

void PlayerEntity::HandleEvents(Event* event)
{
}

bool Entity::WillCollideWithBlock(const v2f& velocity, World* world)
{
	const float twoSixteenths = 2.f / 16.f;
	const float fourteenSixteenths = 14.f / 16.f;

	float minX = position.x + velocity.x + twoSixteenths;
	float maxX = position.x + velocity.x + fourteenSixteenths;

	float minY = position.y + velocity.y + twoSixteenths;
	float maxY = position.y + velocity.y + fourteenSixteenths;

	bool returnValue = true;
	
	if (noclip)
		returnValue = false;
	else if (world->GetChunk(v2f(minX / CHUNK_WIDTH, minY / CHUNK_HEIGHT)) == nullptr)
		returnValue = true;
	else
		returnValue =
		TileRegistry::Tiles.at(world->TileAt(minX, minY, 1))->IsSolidAt(v2f(minX, minY), world, 1)
		|| TileRegistry::Tiles.at(world->TileAt(maxX, minY, 1))->IsSolidAt(v2f(maxX, minY), world, 1)
		|| TileRegistry::Tiles.at(world->TileAt(minX, maxY, 1))->IsSolidAt(v2f(minX, maxY), world, 1)
		|| TileRegistry::Tiles.at(world->TileAt(maxX, maxY, 1))->IsSolidAt(v2f(maxX, maxY), world, 1);

	return returnValue;
}


void PlayerEntity::Update(World* world, float dt)
{
	if (OptionsManager::currentInputType != OptionsManager::InputType::Controller)
	{

		if (OptionsManager::IsForwardsActive())
		{
			m_currentDirection = Direction::North;

			if (WillCollideWithBlock(dt * v2f(0, -1) * 2.f * speed, world))
			{
				velocity.y *= 0.2f;
			}
			else
			{
				velocity.y--;
			}
		}

		if (OptionsManager::IsBackwardsActive())
		{
			m_currentDirection = Direction::South;

			if (WillCollideWithBlock(dt * v2f(0, 1) * 2.f * speed, world))
			{
				velocity.y *= 0.2f;
			}
			else
			{
				velocity.y++;
			}
		}

		if (OptionsManager::IsLeftActive())
		{
			m_currentDirection = Direction::West;

			if (WillCollideWithBlock(dt * v2f(-1, 0) * 2.f * speed, world))
			{
				velocity.x *= 0.2f;
			}
			else
			{
				velocity.x--;
			}
		}

		if (OptionsManager::IsRightActive())
		{
			m_currentDirection = Direction::East;
			if (WillCollideWithBlock(dt * v2f(1, 0) * 2.f * speed, world))
			{
				velocity.x *= 0.2f;
			}
			else
			{
				velocity.x++;
			}
		}
	}
	else {

		float axisX = OptionsManager::GetJoystickAxisX();
		float axisY = OptionsManager::GetJoystickAxisY();

		if (WillCollideWithBlock(dt * v2f(axisX / 100.f, axisY / 100.f) * 4.f, world))
		{
			velocity *= 0.1f;
		}
		else
		{
			velocity += v2f(axisX / 100.f, axisY / 100.f);
		}
	}

	TileID groundTile = static_cast<TileID>(world->TileAt(position.x + .5f, position.y + .5f, 0));
	Tile* tile = TileRegistry::Tiles[groundTile];

	inWater = (groundTile == TileID::Water);

	speed = inWater ? 1.5f : 4.f;
	speed += speedOffset;



	atlasID = tile->useDefaultDirectionalSprites ? DefaultDirectionalSprites[m_currentDirection] : ContextualDirectionalSprites.at(groundTile).at(m_currentDirection);

	position += velocity * dt * (speed / tile->tileProperties.slipperiness);

	velocity *= 0.25f * tile->tileProperties.slipperiness;
}

void PlayerEntity::Draw(sf::RenderWindow* window, v2f cameraPosition)
{
	Entity::Draw(window, cameraPosition);

	Sprite s = Sprite(*Entity::entitiesAtlas, IntRect(v2i(atlasID.x * TILE_SIZE, atlasID.y * TILE_SIZE), v2i(TILE_SIZE, TILE_SIZE)));

	s.setPosition(v2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
	s.setScale(v2f(TEXTURE_SCALE, TEXTURE_SCALE));

	window->draw(s);
}