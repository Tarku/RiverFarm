#pragma once

#include "Tile.h"

class TilledSoilTile : public Tile
{
public:
	TilledSoilTile();

	void OnRandomUpdate(const v2f& position, World* world, int layer = 0) override;
};
