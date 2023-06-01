#pragma once

#include "Shared.h"

enum TileCategory {
	DirtLike
};

struct Tile
{
	std::string name = "Unknown";
	AtlasID id;

	TileCategory tileCategory;
	int itemDrop;
};