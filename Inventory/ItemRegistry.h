#pragma once

#include "Item.h"

class ItemRegistry
{
public:
	static const Item Items[];

	static int GetItemAmount();
};

enum ItemID
{
	ItemAir,
	ItemWood,
	ItemDirt,
	ItemSand
};