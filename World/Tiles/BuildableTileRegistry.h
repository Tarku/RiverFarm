#pragma once

#include "BuildableTile.h"

class BuildableTileRegistry 
{
public:
	static std::vector<BuildableTile> BuildableTiles;

	static int BuildableTileCount();
};