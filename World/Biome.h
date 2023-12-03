#pragma once

#include "Tiles/TileRegistry.h"

struct TreeGenerationProperty
{
	TileID treeType;
	int weight;

	TreeGenerationProperty(TileID treeType, int weight) : treeType(treeType), weight(weight)
	{

	}
};

class Biome
{
public:
	std::string name;

	TileID groundTile;
	TileID waterTile;

	std::vector<TreeGenerationProperty> treeGenerationProperties;

	int treeScarcity;

	Biome(const std::string& name, TileID groundTile, std::vector<TreeGenerationProperty, std::allocator<TreeGenerationProperty>> treeGenerationProperties, int treeScarcity, TileID waterTile = TileID::Water);
};