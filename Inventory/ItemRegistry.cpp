#include "ItemRegistry.h"

std::vector<Item*> ItemRegistry::Items {
	new Item("Wood", {0, 0}),
	new Item("Dirt", {2, 0}),
	new Item("Sand", {1, 0}),
	new Item("Straw", {3, 0}),
	new Item("Flowers", {0, 1}),
	new Item("Barley", {3, 1}),
	new SeedItem("Barley Seeds", {2, 1}, CropID::CropBarley),
	new Item("Milk", {1, 1}),
	new Item("Water", {0, 2}),
		new SeedItem("Glasswort Seeds", {1, 2}, CropID::CropGlasswort),
		new Item("Glasswort", {2, 2})
};

int ItemRegistry::ItemAmount()
{
	return static_cast<int>(ItemRegistry::Items.size());
}