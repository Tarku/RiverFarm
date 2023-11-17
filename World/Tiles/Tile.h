#pragma once

#include "../Shared.h"
#include "../World.h"
#include "../Inventory/ItemID.h"

struct PlantSubstrateProperties
{
	bool isPlantable = false;
	float cropGrowthFactor = 1.0f;

	PlantSubstrateProperties()
	{
			
	}

	PlantSubstrateProperties(bool isPlantable, float cropGrowthFactor = 1.0f)
	{
		this->isPlantable = isPlantable;
		this->cropGrowthFactor = cropGrowthFactor;
	}
};

struct TileProperties
{
	// This determines if the tile can be tilled
	bool isArable = false;
	// This determines if the tile can be dug
	bool isDiggable = false;
	// This determines if the tile can be chopped
	bool isChoppable = false;
	// This determines if the tile is solid or not (aka. a wall)
	bool isSolid = false;
	// This holds values about the tile's quality as a substrate
	PlantSubstrateProperties substrateProperties{};

	// By default, all properties are set to false.
	TileProperties()
	{
		this->isArable = false;
		this->isDiggable = false;
		this->isChoppable = false;
		this->isSolid = false;
		this->substrateProperties = PlantSubstrateProperties();
	}

	TileProperties(bool isArable, bool isDiggable, bool isChoppable, bool isSolid, PlantSubstrateProperties substrateProperties = PlantSubstrateProperties())
	{
		this->isArable = isArable;
		this->isDiggable = isDiggable;
		this->isChoppable = isChoppable;
		this->isSolid = isSolid;
		this->substrateProperties = substrateProperties;
	}
};

class Tile
{
public:
	std::string name{};
	AtlasID textureId{};

	ItemID itemDrop = ItemID::Null;

	TileProperties tileProperties{};

	AtlasID groundId = {0, 0};

	Tile();

	Tile(const std::string& name, const AtlasID& textureId, const ItemID& itemDrop, const TileProperties& tileProperties);

	Tile(const std::string& name, const std::tuple<AtlasID, AtlasID>& textureIds, const ItemID& itemDrop, const TileProperties& tileProperties);

	virtual void OnTileBreak(const v2f& position, World* world, int layer = 0);

	virtual void OnRandomUpdate(const v2f& position, World* world, int layer = 0);

	virtual void OnUpdate(const v2f& position, World* world, int layer = 0);

	virtual AtlasID GetContextTextureID(const v2f& position, World* world, int layer);
	virtual bool IsSolidAt(const v2f& position, World* world, int layer);
};
