#pragma once

#include <vector>
#include "Item.h"

class ItemRegistry
{
public:
	static std::vector<Item*> Items;

	static int GetItemAmount();
};