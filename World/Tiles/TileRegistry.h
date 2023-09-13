#pragma once

#include "Tile.h"
#include "AirTile.h"
#include "TilledSoilTile.h"
#include "WaterTile.h"
#include "TilledSandTile.h"

#include <vector>

class TileRegistry
{
public:
	inline static std::vector<Tile*> Tiles = {
		new AirTile(),
		new Tile("Dirt", AtlasID(1, 0), ItemID::ItemDirt, TileProperties(true, true, false, true)),
		new TilledSoilTile(),
		new Tile("Grass", AtlasID(3, 0), ItemID::ItemDirt, TileProperties(true, true, false, true)),
		new Tile("Mushrub", AtlasID(0, 1), ItemID::ItemWood, TileProperties(false, false, true, true)),
		new Tile("Cactus", AtlasID(4, 2), ItemID::ItemWood, TileProperties(false, false, true, true)),
		new WaterTile(),
		new Tile("Watered Tilled Soil", AtlasID(2, 1), ItemID::ItemDirt, TileProperties(false, true, false, true, PlantSubstrateProperties(true, 1.f))),
		new Tile("Sand", AtlasID(3, 1), ItemID::ItemSand, TileProperties(true, true, false, true)),

		new Tile("Stone", std::make_tuple(AtlasID(5, 2), AtlasID(1, 2)), ItemID::ItemSand, TileProperties(false, false, false, true)),
		new Tile("Gravel", AtlasID(2, 2), ItemID::ItemSand, TileProperties(false, true, false, true)),
		new Tile("Flowers", AtlasID(3, 2), ItemID::ItemFlowers, TileProperties(true, true, false, false)),

		new Tile("Wood Wall", std::make_tuple(AtlasID(0, 3), AtlasID(1, 3)), ItemID::ItemWood, TileProperties(false, false, true, true)),
		new Tile("Stonebrick Wall", std::make_tuple(AtlasID(4, 0), AtlasID(4, 1)), ItemID::Null, TileProperties(false, false, false, true)),

		new TilledSandTile(),
		new Tile("Watered Tilled Sand", AtlasID(5, 1), ItemID::ItemSand, TileProperties(false, true, false, true, PlantSubstrateProperties(true, 0.5f)))

	};

	inline static int TileCount()
	{
		return static_cast<int>(Tiles.size());
	};
};

enum TileID
{
	Air,
	Dirt,
	TilledSoil,
	Grass,
	Shrub,
	Cactus,
	Water,
	WateredTilledSoil,
	Sand,
	Stone,
	Gravel,
	Flowers,
	WoodWall,
	StonebrickWall,
	TilledSand,
	WateredTilledSand
};