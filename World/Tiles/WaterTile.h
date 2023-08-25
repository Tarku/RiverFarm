#pragma once

#include "Tile.h"
#include "../../Shared.h"

class WaterTile : public Tile
{
public:
	WaterTile();

	void OnUpdate(const v2f& position, World* world, int layer = 0) override;
};