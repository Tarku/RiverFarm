#pragma once

#include "Shared.h"
#include "PerlinNoise.hpp"


using namespace sf;

class World
{
private:
	unsigned char m_map[MAP_LEVELS][MAP_HEIGHT][MAP_WIDTH];

	siv::PerlinNoise m_perlin{};

public:
	World();

	void ResetWorld();
	void Update();

	void DoWorldGen();

	void AttemptSpreadWater(const Vector2f& position);

	unsigned char TileAt(const Vector2f& position, int layer);
	void SetTile(const Vector2f& position, int layer, unsigned char tileID);

	bool IsEmptyAt(const Vector2f& position, int layer);
	bool InBounds(const Vector2f& position, int layer);
};

