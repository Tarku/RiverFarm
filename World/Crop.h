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
	bool isHalophyte = false;
	std::string name{};

	float baseGrowthRate{};
	ItemID tileDrops{};
	ItemID seedItemDrop{};
	AtlasID textureID{};

	float growth = 0.0f;
	float actualGrowthRate{};

	bool hasRightSoil = false;
	bool hasWater = false;
	bool isFullyGrown = false;
	bool isGrowing = true;

	virtual void OnUpdate(const v2f& position, std::shared_ptr<Chunk> parentChunk, float dt);

	virtual void OnDestroy(const v2f& position, std::shared_ptr<Chunk> parentChunk, World* world);
};

class CerealCrop : public Crop
{
protected:
	CerealCrop() {};
public:
	void OnDestroy(const v2f& position, std::shared_ptr<Chunk> parentChunk, World* world) override;
};

class BarleyCrop : public CerealCrop
{
public:
	BarleyCrop();

};

class GlasswortCrop : public Crop
{
public:
	GlasswortCrop();
};