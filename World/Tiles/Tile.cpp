#include "Tile.h"
#include "TileRegistry.h"

Tile::Tile()
{

}

Tile::Tile(const std::string& name, const AtlasID& textureId, std::vector<TileDrop> tileDrops, const TileProperties& tileProperties, bool useDefaultDirectionalSprites)
{
	this->name = name;
	this->textureId = textureId;
	this->tileDrops = tileDrops;
	this->tileProperties = tileProperties;

	this->groundId = this->textureId;
	this->useDefaultDirectionalSprites = useDefaultDirectionalSprites;
}

Tile::Tile(const std::string& name, const std::tuple<AtlasID, AtlasID>& textureIds, std::vector<TileDrop> tileDrops, const TileProperties& tileProperties, bool useDefaultDirectionalSprites)
{
	this->name = name;
	this->tileDrops = tileDrops;
	this->tileProperties = tileProperties;

	this->textureId = std::get<0>(textureIds);
	this->groundId = std::get<1>(textureIds);

	this->useDefaultDirectionalSprites = useDefaultDirectionalSprites;
}

void Tile::TryTileBreak(const v2f& position, World* world, int layer)
{
	Metadata meta = world->MetaAt(position, layer);

	if (meta.damage == 0xf)
	{
		OnTileBreak(position, world, layer);
	}
}

void Tile::OnTileBreak(const v2f& position, World* world, int layer)
{
	world->SetTile(position, layer, TileID::Air, true);
	TileRegistry::Tiles[TileID::Air]->OnUpdate(position, world, layer);

	for (auto& neighbor : TileNeighbors)
	{
		TileRegistry::Tiles[world->TileAt(position + neighbor, 0)]->OnUpdate(position, world, 0);
		TileRegistry::Tiles[world->TileAt(position + neighbor, 1)]->OnUpdate(position, world, 1);
	}

	if (tileDrops.size() != 0)
	{
		ItemID itemDrop = ItemID::Null;

		int amount = 0;
		int totalWeight = 0;

		for (TileDrop& td : tileDrops)
		{
			totalWeight += td.weight;
		}

		for (TileDrop& td : tileDrops)
		{
			float weightedChance = (float) td.weight / (float) totalWeight;

			if (Utils::RandFloat(0.0f, 1.0f) < weightedChance)
			{
				itemDrop = td.itemID;
				amount = Utils::RandInt(td.minCount, td.maxCount);
				break;
			}
		}

		world->AddItemEntity(v2f((int)position.x, (int)position.y), itemDrop, amount);
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

AtlasID Tile::GetContextTextureID(const v2f& position, World* world, int layer)
{
	if (layer == 0)
		return groundId;
	else
		return textureId;
}

bool Tile::IsSolidAt(const v2f& position, World* world, int layer)
{
	return tileProperties.isSolid;
}