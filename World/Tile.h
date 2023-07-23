#pragma once

#include "../Shared.h"
#include "World.h"

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

struct Tile
{
	std::string name = "Unknown";
	AtlasID textureId = { 0, 0 };

	ItemID itemDrop;

	TileProperties tileProperties;

	AtlasID groundId;

	Tile(const std::string& name, const AtlasID& textureId, const ItemID& itemDrop, const TileProperties& tileProperties)
	{
		this->name = name;
		this->textureId = textureId;
		this->itemDrop = itemDrop;
		this->tileProperties = tileProperties;

		this->groundId = textureId;
	}

	Tile(const std::string& name, const std::tuple<AtlasID, AtlasID>& textureIds, const ItemID& itemDrop, const TileProperties& tileProperties)
	{
		this->name = name;
		this->itemDrop = itemDrop;
		this->tileProperties = tileProperties;

		this->textureId = std::get<0>(textureIds);
		this->groundId = std::get<1>(textureIds);
	}

	void OnTileBreak(const v2f& position, World* world, int layer = 0) const
	{
		world->SetTile(position, layer, 0);

		world->AddItemEntity(v2f((int)position.x, (int)position.y), itemDrop);
	}
};