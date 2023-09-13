#include "ToolDeclarations.h"
#include "../Interface.h"

SeedBagTool::SeedBagTool()
{
	uiIcon = { 1, 1 };
	name = "Seed Bag";
}

bool SeedBagTool::CanBeUsedHere(World* world, const sf::Vector2f& position)
{
	auto positions = world->WorldToChunkPosition(position);

	v2f chunkPosition = std::get<0>(positions);
	v2f inChunkOffset = std::get<1>(positions);

	ItemID seedItemID = CropRegistry::Crops[m_currentCrop]->seedItemDrop;

	return
		!world->GetChunk(chunkPosition)->IsCrop(inChunkOffset)
		&& Inventory::GetAmount(seedItemID) > 0
		&& TileRegistry::Tiles[world->TileAt(position, 0)]->tileProperties.substrateProperties.isPlantable;
}

void SeedBagTool::OnUse(World* world, const sf::Vector2f& position)
{
	ItemID seedItemID = CropRegistry::Crops[m_currentCrop]->seedItemDrop;
	SeedItem* seedItem = static_cast<SeedItem*>(ItemRegistry::Items[seedItemID]);

	if (seedItem == nullptr)
		return;

	Inventory::Remove(seedItemID, 1);

	auto positions = world->WorldToChunkPosition(position);

	v2f chunkPosition = std::get<0>(positions);
	v2f inChunkOffset = std::get<1>(positions);

	world->GetChunk(chunkPosition)->AddCrop(inChunkOffset, CropRegistry::GetInstance(seedItem->cropType));
}

void SeedBagTool::OnRightClick(World* world, const v2f& position)
{
	int nextCropId = static_cast<int>(m_currentCrop) + 1;

	if (nextCropId >= CropRegistry::CropAmount())
		nextCropId = 0;

	m_currentCrop = static_cast<CropID>(nextCropId);
}

void SeedBagTool::Draw(Interface* inter)
{
	ItemID seedItemID = CropRegistry::Crops[m_currentCrop]->seedItemDrop;
	SeedItem* seedItem = static_cast<SeedItem*>(ItemRegistry::Items[seedItemID]);
	AgriculturalTool::Draw(inter);

	float seedIconX = 0.5f * WINDOW_WIDTH + UI_ICON_WIDTH / 2.f;
	float seedIconY = 0.f * WINDOW_HEIGHT + UI_ICON_HEIGHT;

	sf::Sprite seedIconSprite =
		sf::Sprite(
			*AtlasManager::GetAtlas(AtlasTextureID::Items),
			IntRect(
				v2i(
					TILE_SIZE * seedItem->atlasID.x,
					TILE_SIZE * seedItem->atlasID.y
				),
				v2i(
					TILE_SIZE,
					TILE_SIZE
				)
			)
		);

	seedIconSprite.setPosition(v2f(seedIconX, seedIconY));
	seedIconSprite.setScale(v2f(2.f, 2.f));
	Interface::window->draw(seedIconSprite);
}
