#pragma once

#include "../Shared.h"
#include "../Utils.h"
#include "../Inventory/ItemID.h"

class Chunk;
class World;

class Crop
{
protected:
	uint m_uuid = Utils::RandInt(1, 0x0FFFFFFF);
	Crop() {};

public:
	std::string name;

	float baseGrowthRate;
	ItemID itemDrop;
	ItemID seedItemDrop;
	AtlasID textureID = AtlasID(2, 3);

	float growth = 0.0f;
	float actualGrowthRate;

	bool isFullyGrown = false;
	bool isGrowing = true;

	virtual void OnUpdate(const v2f& position, Chunk* parentChunk, float dt);

	virtual void OnDestroy(const v2f& position, Chunk* parentChunk, World* world);
};

class CerealCrop : public Crop
{
protected:
	CerealCrop() {};
public:
	void OnDestroy(const v2f& position, Chunk* parentChunk, World* world) override;
};

class BarleyCrop : public CerealCrop
{
public:
	BarleyCrop();

};