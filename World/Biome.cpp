#include "Biome.h"

Biome::Biome(const std::string& name, TileID groundTile, std::vector<TreeGenerationProperty, std::allocator<TreeGenerationProperty>> treeGenerationProperties, int treeScarcity, TileID waterTile)
	: name(name), groundTile(groundTile), treeGenerationProperties(treeGenerationProperties), treeScarcity(treeScarcity), waterTile(waterTile)
{;
}