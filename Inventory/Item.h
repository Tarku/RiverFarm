#pragma once

#include "../Shared.h"
#include "../World/CropRegistry.h"

class Chunk;
class World;

class Item
{
public:

	std::string name{};
	AtlasID atlasID{};

	Item() {};

	Item(const std::string& name, AtlasID atlasID)
	{
		this->name = name;
		this->atlasID = atlasID;
	}
};

class SeedItem : public Item
{
private:
public:
	CropID cropType{};
	SeedItem(const std::string& name, AtlasID atlasID, CropID cropType)
	{
		this->name = name;
		this->atlasID = atlasID;
		this->cropType = cropType;
	}
	//void OnUse(const v2f& position, Chunk* chunk, World* world);
};

