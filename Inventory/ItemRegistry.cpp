#include "ItemRegistry.h"

const Item ItemRegistry::Items[] = {
	Item("Air", {4, 4}),
	Item("Wood", {0, 0}),
	Item("Dirt", {2, 0}),
	Item("Sand", {1, 0})
};

int ItemRegistry::GetItemAmount()
{
	return sizeof(Items) / sizeof(Item);
}