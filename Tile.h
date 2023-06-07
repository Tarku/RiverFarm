#pragma once

#include "Shared.h"

enum TileCategory {
	NonDirtLike,
	DirtLike
};

struct Tile
{
	std::string name = "Unknown";
	AtlasID id = { 0, 0 };

	TileCategory tileCategory = NonDirtLike;
	int itemDrop = -1;
};