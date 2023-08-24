#pragma once

#include "../Shared.h"
#include "../World.h"
#include "../Inventory/ItemID.h"

struct TileProperties
{
	bool isArable = false;
	bool isDiggable = false;
	bool isChoppable = false;
	bool isSolid = false;

	// By default, all properties are set to false.
	TileProperties()
	{
		this->isArable = false;
		this->isDiggable = false;
		this->isChoppable = false;
		this->isSolid = false;
	}

	TileProperties(bool isArable, bool isDiggable, bool isChoppable, bool isSolid)
	{
		this->isArable = isArable;
		this->isDiggable = isDiggable;
		this->isChoppable = isChoppable;
		this->isSolid = isSolid;
	}
};

class Tile
{
public:
	std::string name = "Unknown";
	AtlasID textureId = { 0, 0 };

	ItemID itemDrop = ItemID::Null;

	TileProperties tileProperties;

	AtlasID groundId;

	Tile();

	Tile(const std::string& name, const AtlasID& textureId, const ItemID& itemDrop, const TileProperties& tileProperties);

	Tile(const std::string& name, const std::tuple<AtlasID, AtlasID>& textureIds, const ItemID& itemDrop, const TileProperties& tileProperties);

	virtual void OnTileBreak(const v2f& position, World* world, int layer = 0);

	virtual void OnRandomUpdate(const v2f& position, World* world, int layer = 0);

	virtual void OnUpdate(const v2f& position, World* world, int layer = 0);
};
