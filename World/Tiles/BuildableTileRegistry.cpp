#include "BuildableTileRegistry.h"

#include "TileRegistry.h"
#include "../../Inventory/ItemID.h"

std::vector<BuildableTile> BuildableTileRegistry::BuildableTiles = std::vector<BuildableTile>
{
	BuildableTile(
		0,
		TileID::WoodWall,
		std::vector<BuildableTileIngredient> {
			BuildableTileIngredient(1, ItemID::ItemWood)
		}
	),
	BuildableTile(
		1,
		TileID::WoodWall,
		std::vector<BuildableTileIngredient> {
			BuildableTileIngredient(2, ItemID::ItemWood)
		}
	),
	BuildableTile(
		1,
		TileID::Door,
		std::vector<BuildableTileIngredient> {
			BuildableTileIngredient(2, ItemID::ItemWood)
		}
	),
};

int BuildableTileRegistry::BuildableTileCount()
{
	return static_cast<int>(BuildableTiles.size());
}