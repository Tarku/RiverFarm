#include "ToolDeclarations.h"
#include "../Interface.h"

SeedBagTool::SeedBagTool()
{
	uiIcon = { 1, 1 };
	name = "Seed Bag";
	m_currentSeedItem = static_cast<SeedItem*> (ItemRegistry::Items[ItemID::ItemBarleySeeds]);
}

bool SeedBagTool::CanBeUsedHere(World* world, const sf::Vector2f& position)
{
	return world->TileAt(position, 0) == TileID::TilledSoil || world->TileAt(position, 0) == TileID::WateredTilledSoil;
}

void SeedBagTool::OnUse(World* world, const sf::Vector2f& position)
{
	if (m_currentSeedItem == nullptr)
		return;

	auto positions = world->WorldToChunkPosition(position);

	v2f chunkPosition = std::get<0>(positions);
	v2f inChunkOffset = std::get<1>(positions);

	world->GetChunk(chunkPosition)->AddCrop(inChunkOffset, CropRegistry::GetInstance(m_currentSeedItem->cropType));
}
void SeedBagTool::Draw(Interface* inter)
{
	AgriculturalTool::Draw(inter);

	float seedIconX = 0.5f * WINDOW_WIDTH + UI_ICON_WIDTH / 2.f;
	float seedIconY = 0.f * WINDOW_HEIGHT + UI_ICON_HEIGHT;

	sf::Sprite seedIconSprite =
		sf::Sprite(
			*AtlasManager::GetAtlas(AtlasTextureID::Items),
			IntRect(
				TILE_SIZE * m_currentSeedItem->atlasID.x,
				TILE_SIZE * m_currentSeedItem->atlasID.y,
				TILE_SIZE,
				TILE_SIZE
			)
		);

	seedIconSprite.setPosition(v2f(seedIconX, seedIconY));
	seedIconSprite.setScale(v2f(2.f, 2.f));
	Interface::window->draw(seedIconSprite);
}
