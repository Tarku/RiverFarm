#include "WaterTile.h"
#include "TileRegistry.h"
#include "../../Inventory/ItemID.h"

WaterTile::WaterTile()
{
	this->name = "Water";
	this->textureId = { 1, 1 };
	this->groundId = textureId;
	this->tileProperties = TileProperties();
	this->tileDrops = std::vector<TileDrop>{ };
	useDefaultDirectionalSprites = false;
}


void WaterTile::OnUpdate(const v2f& position, World* world, int layer)
{

	if (layer == 1) return;

	Metadata thisTileMeta = world->MetaAt(position, layer);

	for (auto& neighbor : TileNeighbors)
	{
		if (world->TileAt(position + neighbor, layer) == TileID::Air)
		{
			world->SetTile(position, layer, TileID::Water, false);
			world->SetMeta(position, layer, Metadata(0b01, 0, 0));

			return;
		}
		else if (world->TileAt(position + neighbor, layer) == TileID::Water)
		{

			Metadata neighborTileMeta = world->MetaAt(position + neighbor, layer);

			if (neighborTileMeta.misc < 0b111)
			{
				world->SetMeta(position + neighbor, layer, Metadata(neighborTileMeta.misc + 0b001, 0, 0));
				world->SetMeta(position, layer, Metadata(thisTileMeta.misc - 0b001, 0, 0));
			}

		}
	}

	if (thisTileMeta.misc <= 0)
	{
		world->SetTile(position, layer, TileID::Air, false);
	}
}

void WaterTile::OnRandomUpdate(const v2f& position, World* world, int layer)
{
	if (layer == 1) return;

	Metadata thisTileMeta = world->MetaAt(position, layer);

	for (auto& neighbor : TileNeighbors)
	{
		if (world->TileAt(position + neighbor, layer) == TileID::Air)
		{
			world->SetTile(position, layer, TileID::Water, false);
			world->SetMeta(position, layer, Metadata(0b01, 0, 0));

			return;
		}
		else if (world->TileAt(position + neighbor, layer) == TileID::Water)
		{

			Metadata neighborTileMeta = world->MetaAt(position + neighbor, layer);

			if (neighborTileMeta.misc < 0b111 && neighborTileMeta.misc < thisTileMeta.misc)
			{
				world->SetMeta(position + neighbor, layer, Metadata(neighborTileMeta.misc + 0b001, 0, 0));
				world->SetMeta(position, layer, Metadata(thisTileMeta.misc - 0b001, 0, 0));
			}


		}
	}

	if (thisTileMeta.misc <= 0)
	{
		world->SetTile(position, layer, TileID::Air, false);
	}
}

AtlasID WaterTile::GetContextTextureID(const v2f& position, World* world, int layer)
{
	Metadata thisTileMeta = world->MetaAt(position, layer);
	AtlasID returnID = AtlasID(7 - thisTileMeta.misc, 4);

	return returnID;
}
