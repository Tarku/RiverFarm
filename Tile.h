#pragma once

#include "Shared.h"
#include "World.h"

enum TileCategory {
	NonDirtLike,
	DirtLike
};

struct Tile
{
	std::string name = "Unknown";
	AtlasID id = { 0, 0 };

	TileCategory tileCategory = NonDirtLike;
	ItemID itemDrop = ItemID::ItemAir;

	void OnTileBreak(const v2f& position, World* world, int layer = 0)
	{
		world->SetTile(position, layer, 0);
		world->WorldEntities.push_back(new ItemEntity(position, itemDrop));
	}
};