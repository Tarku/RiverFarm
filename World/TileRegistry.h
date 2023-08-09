#pragma once

#include "Tile.h"

class TileRegistry
{
public:
	static Tile Tiles[];

	static int TileCount();
};

enum TileID
{
	Air,
	Dirt,
	TilledSoil,
	Grass,
	Shrub,
	Water,
	WateredTilledSoil,
	Sand,
	Stone,
	Gravel,
	Flowers,
	WoodWall
};