#include "Item.h"
#include "../World/World.h"
#include "../World/Chunk.h"
#include "../World/Crop.h"

/*
void SeedItem::OnUse(const v2f& position, Chunk* chunk, World* world)
{
	auto positionTuple = world->WorldToChunkPosition(position);

	auto chunkPosition = std::get<0>(positionTuple);
	auto chunkPositionOffset = std::get<1>(positionTuple);

	if (chunk->CropAt(chunkPositionOffset))
		return;

	chunk->AddCrop(chunkPositionOffset, CropRegistry::crops[m_cropType]);
}*/