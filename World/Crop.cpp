#include "Crop.h"
#include "Chunk.h"
#include "World.h"
#include "../Inventory/Inventory.h"
#include "../Inventory/ItemID.h"


void Crop::OnUpdate(Chunk* chunk, float dt)
{
	actualGrowthRate = baseGrowthRate;

	if (growth >= 1)
		isGrowing = false;


	if (isGrowing)
		growth += actualGrowthRate * dt * 2;
	else
		growth = 1.f;
};

void Crop::OnDestroy(const v2f& position, Chunk* parentChunk, World* world)
{
	float worldX = parentChunk->position.x + position.x;
	float worldY = parentChunk->position.y  + position.y;
	printf("crop x : %f, crop y : %f\n", worldX, worldY);

	world->AddItemEntity(v2f(worldX, worldY), itemDrop);
	
	for (int i = 0; i < Utils::RandInt(1, 3); i++)
		world->AddItemEntity(v2f(worldX, worldY), seedItemDrop);

	parentChunk->RemoveCrop(position);
}

void CerealCrop::OnDestroy(const v2f& position, Chunk* parentChunk, World* world)
{
	float worldX = parentChunk->position.x + position.x;
	float worldY = parentChunk->position.y + position.y;

	Crop::OnDestroy(position, parentChunk, world);

	for (int i = 0; i < Utils::RandInt(3, 5); i++)
		world->AddItemEntity(v2f(worldX, worldY), ItemID::ItemStraw);
};


BarleyCrop::BarleyCrop () {
	this->name = std::string("Barley");

	this->baseGrowthRate = 0.01f;

	this->itemDrop = ItemID::ItemBarley;
	this->seedItemDrop = ItemID::ItemBarleySeeds;
}