#include "FlowerTile.h"
#include "TileRegistry.h"

FlowerTile::FlowerTile()
{
	name = "Flowers";
	tileDrops = std::vector<TileDrop>{ TileDrop(1, ItemID::ItemFlowers, 0, 2) };

	tileProperties = TileProperties(false, false, false, false);
}

AtlasID FlowerTile::GetContextTextureID(const v2f& position, World* world, int layer)
{
	uint seed = (short) position.x * (short) position.y;

	srand(seed);

	
	int randomFlower = rand() % 3;

	AtlasID returnID = {9, 0};

	switch (randomFlower)
	{
	case 0:
		returnID = { 9, 0 };
		break;
	case 1:
		returnID = { 10, 0 };
		break;
	case 2:
		returnID = { 11, 0 };
		break;
	}

	return returnID;
}