#include "Crop.h"
#include "Chunk.h"
#include "World.h"
#include "Tiles/Tile.h"
#include "Tiles/TileRegistry.h"
#include "../Inventory/Inventory.h"
#include "../Inventory/ItemID.h"


void Crop::OnUpdate(const v2f& position, Chunk* parentChunk, float dt)
{
	actualGrowthRate = baseGrowthRate;

	uchar groundTileId = parentChunk->TileAt(position, 0);

	hasRightSoil = isHalophyte ? (groundTileId == TileID::WateredTilledSand) : (groundTileId == TileID::WateredTilledSoil);
	hasWater = (groundTileId == TileID::WateredTilledSand) || (groundTileId == TileID::WateredTilledSoil);

	if (!hasRightSoil)
	{
		isGrowing = false;
		return;
	}

	if (growth >= 1 && isGrowing == true)
	{

		isGrowing = false;
		isFullyGrown = true;
	}


	if (isGrowing && !isFullyGrown)
		growth += actualGrowthRate * dt;
	else
		growth = 1.f;
};

void Crop::OnDestroy(const v2f& position, Chunk* parentChunk, World* world)
{
	float worldX = parentChunk->position.x + position.x;
	float worldY = parentChunk->position.y  + position.y;

	world->AddItemEntity(v2f(worldX, worldY), itemDrop);

	world->AddItemEntity(v2f(worldX, worldY), seedItemDrop);

	// 1/5 chance to get an extra seed
	if (Utils::RandInt(0, 99) < 20)
	{
		world->AddItemEntity(v2f(worldX, worldY), seedItemDrop);

		// 1/20 chance to get another extra seed
		// (1/100 chance to get 3 seeds in total)
		if (Utils::RandInt(0, 99) < 5)
		{
			world->AddItemEntity(v2f(worldX, worldY), seedItemDrop);
		}
	}

	parentChunk->RemoveCrop(position);
}

void CerealCrop::OnDestroy(const v2f& position, Chunk* parentChunk, World* world)
{
	float worldX = parentChunk->position.x + position.x;
	float worldY = parentChunk->position.y + position.y;

	for (int i = 0; i < Utils::RandInt(0, 2); i++)
		world->AddItemEntity(v2f(worldX, worldY), ItemID::ItemStraw);

	Crop::OnDestroy(position, parentChunk, world);

};


BarleyCrop::BarleyCrop () {
	this->name = std::string("Barley");

	this->baseGrowthRate = 0.05f;
	this->textureID = { 2, 3 };

	this->itemDrop = ItemID::ItemBarley;
	this->seedItemDrop = ItemID::ItemBarleySeeds;
	this->isHalophyte = false;
}

GlasswortCrop::GlasswortCrop()
{
	this->name = std::string("Glasswort");

	this->baseGrowthRate = 0.028f;
	this->textureID = { 3, 3 };

	this->itemDrop = ItemID::ItemGlasswort;
	this->seedItemDrop = ItemID::ItemGlasswortSeeds;

	this->isHalophyte = true;
}