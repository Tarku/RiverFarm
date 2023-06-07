#pragma once

#include "Shared.h"

#include "ItemRegistry.h"

#include <array>

typedef unsigned short amount_t;

class Inventory
{
private:
	static std::array<amount_t, MAX_ITEMS> m_inventoryItems;

public:
	static void Initialize();

	static bool IsIDInBounds(ItemID item);

	static amount_t GetAmount(ItemID item);

	static void Add(ItemID item, amount_t amount);
	static void Remove(ItemID item, amount_t amount);

	static void ShowContents();
};

