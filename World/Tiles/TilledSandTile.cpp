#include "TilledSandTile.h"
#include "TileRegistry.h"

TilledSandTile::TilledSandTile()
{
	this->name = "Tilled Sand";
	this->textureId = AtlasID(5, 0);
	this->tileDrops = std::vector<TileDrop>{ TileDrop(1, ItemID::ItemSand, 1, 1) };
	this->tileProperties = TileProperties(false, true, false, true, PlantSubstrateProperties(true, 0.05f));

	this->groundId = textureId;
}
void TilledSandTile::OnRandomUpdate(const v2f& position, World* world, int layer)
{
	for (auto& neighbor : neighbors)
	{
		if (world->TileAt(position + neighbor, 0) == TileID::Water)
		{
			world->SetTile(position, layer, TileID::WateredTilledSand);
		}
	}
}