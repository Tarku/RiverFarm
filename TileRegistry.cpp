#include "TileRegistry.h"

Tile TileRegistry::Tiles[] = {
	Tile("Air", {0, 0}, TileCategory::NonDirtLike),
	Tile("Dirt", {1, 0}, TileCategory::DirtLike),
	Tile("Tilled Soil", {2, 0}, TileCategory::DirtLike),
	Tile("Grass", {3, 0}, TileCategory::DirtLike),
	Tile("Shrub", {0, 1}, TileCategory::NonDirtLike),
	Tile("Water", {1, 1}, TileCategory::NonDirtLike),
	Tile("Watered Tilled Soil", {2, 1}, TileCategory::DirtLike),
	Tile("Sand", {3, 1}, TileCategory::DirtLike)
};

constexpr unsigned int TileRegistry::TileCount()
{
	return sizeof(TileRegistry::Tiles) / sizeof(Tile);
}
