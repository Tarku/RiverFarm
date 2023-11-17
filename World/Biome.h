#pragma once

#include "Tiles/TileRegistry.h"

class Biome
{
public:
	std::string name;

	TileID groundTile;
	TileID treeTile;

	int treeScarcity;

	Biome(const std::string& name, TileID groundTile, TileID treeTile, int treeScarcity);
};