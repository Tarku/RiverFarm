#pragma once

#include "../../Shared.h"
#include "Tile.h"

class AirTile : public Tile
{
public:
	AirTile();

	void OnUpdate(const v2f& position, World* world, int layer = 0) override;
};