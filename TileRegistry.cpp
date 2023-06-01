#include "TileRegistry.h"

Tile TileRegistry::Tiles[] = {
	Tile("Air", {0, 0}),
	Tile("Dirt", {1, 0}),
	Tile("Tilled Soil", {2, 0}),
	Tile("Grass", {3, 0}),
	Tile("Shrub", {0, 1}),
	Tile("Water", {1, 1}),
	Tile("Watered Tilled Soil", {2, 1}),
	Tile("Sand", {3, 1})
};

constexpr unsigned int TileRegistry::TileCount()
{
	return sizeof(TileRegistry::Tiles) / sizeof(Tile);
}
