#include "BuildableTile.h"

#include "TileRegistry.h"
#include "../../Inventory/ItemID.h"
#include "../../Inventory/Inventory.h"
#include "../World.h"

BuildableTileIngredient::BuildableTileIngredient(
	int cost,
	ItemID itemID
) : cost(cost), itemID(itemID)
{

}

BuildableTile::BuildableTile(
	string name,
	int tileLayer,
	TileID tileID,
	std::vector<BuildableTileIngredient> tileIngredients
) : name(name), tileLayer(tileLayer), tileID(tileID), tileIngredients(tileIngredients)
{

}

bool BuildableTile::CanBePlacedHere(const v2f& position, World* world)
{
	bool condition1 = world->TileAt(position, tileLayer) == TileID::Air;

	bool condition1_bis = tileLayer == 0 && world->TileAt(position, tileLayer) != TileID::Water && world->TileAt(position, tileLayer) != tileID;

	int counter = 0;

	for (BuildableTileIngredient ingredient : tileIngredients)
	{
		// Checking if every ingredient of a tile is in sufficient quantity in inventory
		
		if (Inventory::GetAmount(ingredient.itemID) >= ingredient.cost)
		{
			counter++;
		}
	}

	bool condition2 = counter == tileIngredients.size();

	return (condition1 || condition1_bis) && condition2;
}
