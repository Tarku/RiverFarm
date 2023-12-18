#include "DoorTile.h"
#include "TileRegistry.h"

std::map<long long, bool> DoorTile::DoorStates;

DoorTile::DoorTile()
{
	this->textureId = { 5, 3 };
	this->groundId = { 5, 3 };

	this->tileDrops = std::vector<TileDrop>{ TileDrop(1, ItemID::ItemWood, 1, 3) };
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

bool DoorTile::IsOpenAt(const v2f& position, World* world, int layer)
{
	auto chunkPosTuple = world->WorldToChunkPosition(position);

	v2f chunkWorldPos = std::get<0>(chunkPosTuple);
	v2f inChunkPos = std::get<1>(chunkPosTuple);

	return world->GetChunk(chunkWorldPos)->MetaAt(inChunkPos, layer).binaryState;
}

void DoorTile::SetOpen(const v2f& position, World* world, int layer, bool state)
{
	auto chunkPosTuple = world->WorldToChunkPosition(position);

	v2f chunkWorldPos = std::get<0>(chunkPosTuple);
	v2f inChunkPos = std::get<1>(chunkPosTuple);

	Metadata meta = world->GetChunk(chunkWorldPos)->MetaAt(inChunkPos, layer);
	meta.binaryState = state;

	world->GetChunk(chunkWorldPos)->SetMeta(inChunkPos, layer, meta);
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
		contextId = IsOpenAt(position, world, layer) ? AtlasID(7, 3) : AtlasID(5, 3);
	}
	if (isVerticalDoor)
	{
		contextId = IsOpenAt(position, world, layer) ? AtlasID(5, 3) : AtlasID(6, 3);
	}

	return contextId;
}

bool DoorTile::IsSolidAt(const v2f& position, World* world, int layer)
{
	return !IsOpenAt(position, world, layer);
}