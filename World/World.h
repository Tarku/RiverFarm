#pragma once

#ifndef WORLD_H
#define WORLD_H

#include "../Shared.h"
#include "../Includes/PerlinNoise.hpp"
#include "../Entities/EntityDeclarations.h"

#include "Chunk.h"

class World
{
private:
	Chunk* m_map[MAP_HEIGHT / CHUNK_HEIGHT][MAP_WIDTH / CHUNK_WIDTH] = {};

	siv::PerlinNoise m_perlin{};

public:
	World();

	static std::vector<Entity*> WorldEntities;

	void ResetWorld();
	void Update();

	void Dispose();

	int DrawChunks(RenderWindow* window, const v2f& cameraPosition);

	void DoWorldGen();

	void AttemptSpreadWater(const v2f& position);

	unsigned char TileAt(const v2f& position, int layer);
	void SetTile(const v2f& position, int layer, unsigned char tileID);

	bool IsEmptyAt(const v2f& position, int layer);
	bool InBounds(const v2f& position, int layer);
};

#endif