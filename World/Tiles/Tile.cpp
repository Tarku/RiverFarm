#include "Tile.h"
#include "TileRegistry.h"

Tile::Tile()
{

}

Tile::Tile(const std::string& name, const AtlasID& textureId, const ItemID& itemDrop, const TileProperties& tileProperties)
{
	this->name = name;
	this->textureId = textureId;
	this->itemDrop = itemDrop;
	this->tileProperties = tileProperties;

	this->groundId = textureId;
}

Tile::Tile(const std::string& name, const std::tuple<AtlasID, AtlasID>& textureIds, const ItemID& itemDrop, const TileProperties& tileProperties)
{
	this->name = name;
	this->itemDrop = itemDrop;
	this->tileProperties = tileProperties;

	this->textureId = std::get<0>(textureIds);
	this->groundId = std::get<1>(textureIds);
}

void Tile::OnTileBreak(const v2f& position, World* world, int layer)
{
	world->SetTile(position, layer, TileID::Air, true);
	TileRegistry::Tiles[TileID::Air]->OnUpdate(position, world, layer);

	for (auto& neighbor : neighbors)
	{
		TileRegistry::Tiles[world->TileAt(position + neighbor, 0)]->OnUpdate(position, world, 0);
		TileRegistry::Tiles[world->TileAt(position + neighbor, 1)]->OnUpdate(position, world, 1);
	}

	if (itemDrop != -1)
	{
		world->AddItemEntity(v2f((int)position.x, (int)position.y), itemDrop);
	}
}

void Tile::OnRandomUpdate(const v2f& position, World* world, int layer)
{
	// Base tile doesn't have any update stuff
};

void Tile::OnUpdate(const v2f& position, World* world, int layer)
{
	// Base tile doesn't have any update stuff
};