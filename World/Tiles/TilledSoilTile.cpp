#include "TilledSoilTile.h"
#include "TileRegistry.h"

TilledSoilTile::TilledSoilTile()
{
	this->name = "Tilled Soil";
	this->textureId = AtlasID(2, 0);
	this->itemDrop = ItemID::ItemDirt;
	this->tileProperties = TileProperties(false, true, false, true, PlantSubstrateProperties(true, 0.15f));



	this->groundId = textureId;
}

void TilledSoilTile::OnRandomUpdate(const v2f& position, World* world, int layer)
{
	for (auto& neighbor : neighbors)
	{
		if (world->TileAt(position + neighbor, 0) == TileID::Water)
		{
			world->SetTile(position, layer, TileID::WateredTilledSoil);
		}
	}
}