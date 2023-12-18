#pragma once

#include "Tile.h"
#include "../../Shared.h"

class WaterTile : public Tile
{
public:
	WaterTile();

	void OnRandomUpdate(const v2f& position, World* world, int layer = 0) override;
	void OnUpdate(const v2f& position, World* world, int layer = 0) override;
	AtlasID GetContextTextureID(const v2f& position, World* world, int layer) override;
};