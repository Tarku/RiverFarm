#pragma once

#include "Tile.h"

class TilledSandTile : public Tile
{
public:
	TilledSandTile();

	void OnRandomUpdate(const v2f& position, World* world, int layer = 0) override;
};