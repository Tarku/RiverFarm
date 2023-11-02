#pragma once

#ifndef WORLD_H
#define WORLD_H

#include <list>

#include "../Shared.h"
#include "../Includes/PerlinNoise.hpp"
#include "../Entities/EntityDeclarations.h"

#include "Chunk.h"
#include "WorldTime.h"

class World
{
private:

public:
	World();

	std::vector<Chunk*> Chunks{};

	void DoWorldGen();
	void ResetWorld();

	void Update(const v2f& cameraPosition, float dt);

	WorldTime worldTime;

	static std::list<Entity*> WorldEntities;
	static std::list<Entity*> EntitiesToDelete;

	void AddDecorations();

	std::tuple<v2f, v2f> WorldToChunkPosition(const v2f& worldPosition);
	v2f ChunkToWorldPosition(const v2f& chunkPosition, const v2f& inChunkPosition);

	Chunk* GetChunk(const v2f& position);
	int DrawChunks(RenderWindow* window, const v2f& cameraPosition, bool drawChunkBorders);

	void UpdateChunkList();

	void SaveWorldToImage();

	// * Checks if the tile at position (in world coordinates) is air (TileID::Air, ID: 0)
	bool IsEmptyAt(const v2f& position, int layer);

	// * Checks if the position (in world coordinates) is inside the world's chunks
	bool InBounds(const v2f& position, int layer);

	// Gets the tile at position (in world coordinates)
	unsigned char TileAt(const v2f& position, int layer);

	// Gets the tile at position (in world coordinates)
	unsigned char TileAt(int x, int y, int layer);

	void SetTile(const v2f& position, int layer, unsigned char tileID, bool updateBlocks = false);

	void AddItemEntity(const v2f& position, ItemID itemID, int amount = 1);

	void AddEntity(Entity* entity);
	void RemoveEntity(Entity* entity);

	void Dispose();
};

#endif