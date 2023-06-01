#pragma once

#include "Item.h"

class ItemRegistry
{
public:
	static Item Items[256];

	constexpr static unsigned int ItemCount() { return sizeof(Items) / sizeof(Item); };
};

enum ItemID
{
	ItemWood,
	ItemDirt,
	ItemSand
};