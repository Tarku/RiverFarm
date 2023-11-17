#include "Biome.h"

Biome::Biome(const std::string& name, TileID groundTile, TileID treeTile, int treeScarcity)
	: name(name), groundTile(groundTile), treeTile(treeTile), treeScarcity(treeScarcity)
{;
}