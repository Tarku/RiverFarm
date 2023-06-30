#pragma once

#include "Item.h"

class ItemRegistry
{
public:
	static const Item Items[256];
};

enum ItemID
{
	ItemAir,
	ItemWood,
	ItemDirt,
	ItemSand
};