#pragma once

#include "Tile.h"

class FlowerTile : public Tile
{
public:
	FlowerTile();

	AtlasID GetContextTextureID(const v2f& position, World* world, int layer) override;
};