#pragma once

#include "Tile.h"

class DoorTile : public Tile
{
private:
	static std::map<long long, bool> DoorStates;
public:
	DoorTile();

	// This returns a unique ID for any position (in world coordinates)
	static long long DoorIDAt(const v2f& position);

	static bool IsOpenAt(const v2f& position, World* world, int layer);
	static void SetOpen(const v2f& position, World* world, int layer, bool state);

	AtlasID GetContextTextureID(const v2f& position, World* world, int layer) override;
	bool IsSolidAt(const v2f& position, World* world, int layer) override;
};