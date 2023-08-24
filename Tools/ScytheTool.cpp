#include "ToolDeclarations.h"
#include "../World/Crop.h"

ScytheTool::ScytheTool()
{
	name = "Scythe";
	description = "Is used to harvest plants.";

	uiIcon = { 0, 1 };
}

bool ScytheTool::CanBeUsedHere(World* world, const sf::Vector2f& position)
{
	auto chunkPositionTuple = world->WorldToChunkPosition(position);

	auto chunkPosition = std::get<0>(chunkPositionTuple);
	auto chunkPositionOffset = std::get<1>(chunkPositionTuple);

	return world->TileAt(position, 1) == TileID::Flowers || (world->GetChunk(chunkPosition)->IsCrop(chunkPositionOffset) && !world->GetChunk(chunkPosition)->CropAt(chunkPositionOffset)->isGrowing);
}

void ScytheTool::OnUse(World* world, const sf::Vector2f& position)
{
	auto chunkPositionTuple = world->WorldToChunkPosition(position);

	auto chunkPosition = std::get<0>(chunkPositionTuple);
	auto chunkPositionOffset = std::get<1>(chunkPositionTuple);

	Crop* crop = world->GetChunk(chunkPosition)->CropAt(chunkPositionOffset);

	if (crop != nullptr &&  crop->isFullyGrown)
	{
		crop->OnDestroy(chunkPositionOffset, world->GetChunk(chunkPosition), world);
	}
	else
	{
		TileRegistry::Tiles[world->TileAt(position, 1)]->OnTileBreak(position, world, 1);
	}
}