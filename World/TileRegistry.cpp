#include "TileRegistry.h"

const Tile TileRegistry::Tiles[] = {
	Tile("Air", {0, 0}, TileCategory::NonDirtLike),
	Tile("Dirt", {1, 0}, TileCategory::DirtLike, ItemID::ItemDirt),
	Tile("Tilled Soil", {2, 0}, TileCategory::DirtLike, ItemID::ItemDirt),
	Tile("Grass", {3, 0}, TileCategory::DirtLike, ItemID::ItemDirt),
	Tile("Shrub", {0, 1}, TileCategory::NonDirtLike, ItemID::ItemWood),
	Tile("Water", {1, 1}, TileCategory::NonDirtLike),
	Tile("Watered Tilled Soil", {2, 1}, TileCategory::DirtLike, ItemID::ItemDirt),
	Tile("Sand", {3, 1}, TileCategory::DirtLike, ItemID::ItemSand),

	Tile("Stone", {1, 2}, TileCategory::NonDirtLike, ItemID::ItemSand),
	Tile("Gravel", {2, 2}, TileCategory::DirtLike, ItemID::ItemSand),
	Tile("Grass with flowers", {3, 2}, TileCategory::DirtLike, ItemID::ItemDirt),
};