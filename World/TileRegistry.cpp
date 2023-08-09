#include "TileRegistry.h"

Tile TileRegistry::Tiles[] = {
	Tile("Air", AtlasID(0, 0), (ItemID) (-1), TileProperties()),
	Tile("Dirt", AtlasID(1, 0), ItemID::ItemDirt, TileProperties(true, true, false, true)),
	Tile("Tilled Soil", AtlasID(2, 0), ItemID::ItemDirt, TileProperties(false, true, false, true)),
	Tile("Grass", AtlasID(3, 0), ItemID::ItemDirt, TileProperties(true, true, false, true)),
	Tile("Mushrub", AtlasID(0, 1), ItemID::ItemWood, TileProperties(false, false, true, true)),
	Tile("Water", AtlasID(1, 1), (ItemID)(-1), TileProperties(false, false, false, true)),
	Tile("Watered Tilled Soil", AtlasID(2, 1), ItemID::ItemDirt, TileProperties(false, true, false, true)),
	Tile("Sand", AtlasID(3, 1), ItemID::ItemSand, TileProperties(false, true, false, true)),

	Tile("Stone", AtlasID(1, 2), ItemID::ItemSand, TileProperties(false, false, false, true)),
	Tile("Gravel", AtlasID(2, 2), ItemID::ItemSand, TileProperties(false, true, false, true)),
	Tile("Flowers", AtlasID(3, 2), ItemID::ItemFlowers, TileProperties(true, true, false, false)),

	Tile("Wood Wall", std::make_tuple(AtlasID(0, 3), AtlasID(1, 3)), ItemID::ItemWood, TileProperties(false, false, true, true))
};

int TileRegistry::TileCount()
{
	return sizeof(Tiles) / sizeof(Tile);
}