#pragma once

#include "../Shared.h"

#include "ItemRegistry.h"
#include "Item.h"

#include <array>

typedef unsigned short amount_t;

class ItemRegistry;

class Inventory
{
private:
	static const int m_inventorySize = 256;
	static std::array<amount_t, m_inventorySize> m_inventoryItems;

public:
	static void Initialize();

	static bool IsIDInBounds(ItemID item);

	static amount_t GetAmount(ItemID item);

	static void Add(ItemID item, amount_t amount);
	static void Remove(ItemID item, amount_t amount);

	static void ShowContents();
};

