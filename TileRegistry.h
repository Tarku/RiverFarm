#pragma once

#include "Tile.h"

class TileRegistry
{
public:
	static Tile Tiles[MAX_TILES];

	constexpr static unsigned int TileCount();
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
	GrassWithFlowers
};