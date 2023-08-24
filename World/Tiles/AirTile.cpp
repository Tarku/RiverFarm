#include "AirTile.h"
#include "TileRegistry.h"

AirTile::AirTile()
{
	this->name = "Air";
	this->textureId = { 0, 0 };
	this->itemDrop = ItemID::Null;
	this->tileProperties = TileProperties();
}

void AirTile::OnUpdate(const v2f& position, World* world, int layer)
{
	if (layer == 1) return;

	for (auto& neighbor : neighbors)
	{
		if (world->TileAt(position + neighbor, layer) == TileID::Water)
		{
			world->SetTile(position, layer, TileID::Water);
		}
	}
}