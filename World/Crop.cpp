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

	if (groundTileId != TileID::TilledSoil && groundTileId != TileID::WateredTilledSoil)
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
		growth += actualGrowthRate * dt * 2;
	else
		growth = 1.f;
};

void Crop::OnDestroy(const v2f& position, Chunk* parentChunk, World* world)
{
	float worldX = parentChunk->position.x + position.x;
	float worldY = parentChunk->position.y  + position.y;

	world->AddItemEntity(v2f(worldX, worldY), itemDrop);

	world->AddItemEntity(v2f(worldX, worldY), seedItemDrop);

	if (Utils::RandInt(0, 99) < 20)
	{

		world->AddItemEntity(v2f(worldX, worldY), seedItemDrop);

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

	Crop::OnDestroy(position, parentChunk, world);

	for (int i = 0; i < Utils::RandInt(1, 3); i++)
		world->AddItemEntity(v2f(worldX, worldY), ItemID::ItemStraw);
};


BarleyCrop::BarleyCrop () {
	this->name = std::string("Barley");

	this->baseGrowthRate = 0.05f;

	this->itemDrop = ItemID::ItemBarley;
	this->seedItemDrop = ItemID::ItemBarleySeeds;
}