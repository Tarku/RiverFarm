#pragma once

#include "Tile.h"
#include "AirTile.h"
#include "TilledSoilTile.h"
#include "WaterTile.h"
#include "TilledSandTile.h"
#include "DoorTile.h"
#include "FlowerTile.h"

#include <vector>

class TileDrop
{
public:
	int weight;
	ItemID itemID;
	int minCount;
	int maxCount;

	TileDrop(
		int weight,
		ItemID itemID,
		int minCount,
		int maxCount
	) : weight(weight), itemID(itemID), minCount(minCount), maxCount(maxCount)
	{

	}

};

class TileRegistry
{
public:
	inline static TileProperties DefaultTreeProperties = TileProperties(false, false, true, true);
	inline static TileProperties DefaultGroundProperties = TileProperties(true, true, false, true);

	inline static std::vector<Tile*> Tiles = {
		new AirTile(),
		new Tile("Dirt", AtlasID(1, 0), std::vector<TileDrop> {TileDrop(1, ItemID::ItemDirt, 1, 1)}, DefaultGroundProperties),
		new TilledSoilTile(),
		new Tile("Grass", AtlasID(3, 0), std::vector<TileDrop> {TileDrop(1, ItemID::ItemDirt, 1, 1)}, DefaultGroundProperties),
		new Tile("Tree", AtlasID(0, 1), std::vector<TileDrop> {TileDrop(1, ItemID::ItemWood, 1, 3)}, TileProperties(false, false, true, true)),
		new Tile("Cactus", AtlasID(4, 2), std::vector<TileDrop> {TileDrop(1, ItemID::ItemWood, 1, 2)}, TileProperties(false, false, true, true)),
		new WaterTile(),
		new Tile("Watered tilled soil", AtlasID(2, 1), std::vector<TileDrop> {TileDrop(1, ItemID::ItemDirt, 1, 1)}, TileProperties(false, true, false, true, PlantSubstrateProperties(true, 1.f))),
		new Tile("Sand", AtlasID(3, 1), std::vector<TileDrop> {TileDrop(1, ItemID::ItemSand, 1, 1)}, DefaultGroundProperties),

		new Tile("Stone", std::make_tuple(AtlasID(5, 2), AtlasID(1, 2)), std::vector<TileDrop> {TileDrop(1, ItemID::ItemSand, 1, 1)}, TileProperties(false, false, false, true)),
		new Tile("Gravel", AtlasID(2, 2), std::vector<TileDrop> {TileDrop(1, ItemID::ItemSand, 1, 1)}, TileProperties(false, true, false, true)),
		new FlowerTile(),
		new Tile("Wood planks", std::make_tuple(AtlasID(0, 3), AtlasID(1, 3)), std::vector<TileDrop> {TileDrop(1, ItemID::ItemWood, 2, 2)}, TileProperties(false, false, true, true)),
		new Tile("Stone brick", std::make_tuple(AtlasID(4, 0), AtlasID(4, 1)), std::vector<TileDrop> {}, TileProperties(false, false, false, true)),

		new TilledSandTile(),
		new Tile("Watered tilled sand", AtlasID(5, 1), std::vector<TileDrop> {TileDrop(1, ItemID::ItemSand, 1, 1)}, TileProperties(false, true, false, true, PlantSubstrateProperties(true, 0.5f)), true),

		new Tile("Snow", AtlasID(6, 0), std::vector<TileDrop> {TileDrop(1, ItemID::ItemDirt, 1, 1)}, DefaultGroundProperties),
		new Tile("Swamp grass", AtlasID(7, 0), std::vector<TileDrop> {TileDrop(1, ItemID::ItemDirt, 1, 1)}, DefaultGroundProperties),


		new Tile("Fir tree", AtlasID(6, 1),  std::vector<TileDrop> {TileDrop(1, ItemID::ItemWood, 2, 4)}, DefaultTreeProperties),
		new Tile("Acacia tree", AtlasID(7, 2),  std::vector<TileDrop> {TileDrop(1, ItemID::ItemWood, 1, 3)}, DefaultTreeProperties),
		new Tile("Mangrove tree", AtlasID(6, 2), std::vector<TileDrop> {TileDrop(1, ItemID::ItemWood, 1, 3)}, DefaultTreeProperties),
		new Tile("Dry grass", AtlasID(7, 1),  std::vector<TileDrop> {TileDrop(1, ItemID::ItemDirt, 1, 1)}, TileProperties(true, true, false, false, PlantSubstrateProperties(true, 0.5f))),

		new DoorTile(),

		new Tile("Birch tree", AtlasID(8, 2),  std::vector<TileDrop> {TileDrop(1, ItemID::ItemWood, 1, 3)}, DefaultTreeProperties),
		new Tile("Ice", AtlasID(8, 0), std::vector<TileDrop>{}, TileProperties(false, true, false, false, PlantSubstrateProperties(), 3.0f), false),
	
		new Tile("Ashes", AtlasID(3, 2), std::vector<TileDrop> {}, TileProperties(false, false, false, false, PlantSubstrateProperties(), 1.0f), true)
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
	Tree,
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
	WateredTilledSand,
	Snow,
	SwampGrass,
	FirTree,
	AcaciaTree,
	MangroveTree,
	DryGrass,
	Door,
	BirchTree,
	Ice,
	Ashes
};

