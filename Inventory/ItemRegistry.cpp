#include "ItemRegistry.h"

std::vector<Item*> ItemRegistry::Items {
	new Item("Wood", {0, 0}),
	new Item("Dirt", {2, 0}),
	new Item("Sand", {1, 0}),
	new Item("Straw", {3, 0}),
	new Item("Flowers", {0, 1}),
	new Item("Barley", {3, 1}),
	new SeedItem("Barley Seeds", {2, 1}, CropID::CropBarley)
};

int ItemRegistry::ItemAmount()
{
	return static_cast<int>(ItemRegistry::Items.size());
}