#include "WaterTile.h"
#include "TileRegistry.h"
#include "../../Inventory/ItemID.h"

WaterTile::WaterTile()
{
	this->name = "Water";
	this->textureId = { 1, 1 };
	this->groundId = textureId;
	this->tileProperties = TileProperties();
	this->itemDrop = ItemID::Null;
}

void WaterTile::OnUpdate(const v2f& position, World* world, int layer)
{

	if (layer == 1) return;

	for (auto& neighbor : neighbors)
	{
		if (world->TileAt(position + neighbor, layer) == TileID::Air)
		{
			world->SetTile(position, layer, TileID::Water, false);
			return;
		}
	}
}