#include "DoorTile.h"
#include "TileRegistry.h"

std::map<long long, bool> DoorTile::DoorStates;

DoorTile::DoorTile()
{
	this->textureId = { 5, 3 };
	this->groundId = { 5, 3 };
	this->tileProperties = TileProperties(false, false, true, true);

	this->name = "Door";
}

long long DoorTile::DoorIDAt(const v2f& position)
{
	int64_t result = 0;

	int32_t x = (int32_t)position.x;
	int32_t y = (int32_t)position.y;

	result = ((int64_t) x << 32) + y;

	//Utils::Log(std::format("x: {}, y: {}; key: {}", x, y, result));

	return result;
}

bool DoorTile::IsOpenAt(const v2f& position)
{
	bool result = false;

	long long id = DoorIDAt(position);

	if (DoorStates.contains(id))
	{
		result = DoorStates.at(id);
	}

	return result;
}

void DoorTile::SetOpen(const v2f& position, bool state)
{

	long long id = DoorIDAt(position);

	if (DoorStates.contains(id))
	{
		DoorStates[id] = state;
	}
	else
	{
		DoorStates.insert(std::make_pair(id, state));
	}
}


AtlasID DoorTile::GetContextTextureID(const v2f& position, World* world, int layer)
{

	// Indices

	// 0 1 2
	// 3 x 4
	// 5 6 7
	std::vector<uchar> neighboringTiles = std::vector<uchar>();
	int index = 0;

	for (int j = -1; j <= 1; j++)
	{
		for (int i = -1; i <= 1; i++)
		{
			if (i == 0 && j == 0)
				continue;

			neighboringTiles.push_back(world->TileAt(v2f((int) (position.x + i), (int) (position.y + j)), layer));
			index++;

		}
	}

	AtlasID contextId = textureId;

	bool isVerticalDoor

		= TileRegistry::Tiles.at(neighboringTiles[1])->tileProperties.isSolid
		&& TileRegistry::Tiles.at(neighboringTiles[6])->tileProperties.isSolid;


	bool isHorizontalDoor

		= TileRegistry::Tiles.at(neighboringTiles[3])->tileProperties.isSolid
		&& TileRegistry::Tiles.at(neighboringTiles[4])->tileProperties.isSolid;


	if (isHorizontalDoor)
	{
		contextId = IsOpenAt(position) ? AtlasID(7, 3) : AtlasID(5, 3);
	}
	if (isVerticalDoor)
	{
		contextId = IsOpenAt(position) ? AtlasID(5, 3) : AtlasID(6, 3);
	}

	return contextId;
}

bool DoorTile::IsSolidAt(const v2f& position, World* world, int layer)
{
	return !IsOpenAt(position);
}