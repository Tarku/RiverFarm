#pragma once

#include "Tile.h"

class TileRegistry
{
public:
	const static Tile Tiles[MAX_TILES];
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