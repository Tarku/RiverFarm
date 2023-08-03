#include "Inventory.h"

std::array<amount_t, Inventory::m_inventorySize> Inventory::m_inventoryItems = std::array<amount_t, Inventory::m_inventorySize>();

void Inventory::Initialize()
{
}

bool Inventory::IsIDInBounds(ItemID item)
{
	return item >= 0 && item < MAX_ITEMS;
}

amount_t Inventory::GetAmount(ItemID item)
{
	return IsIDInBounds(item) ? m_inventoryItems[item] : -1;
}

void Inventory::Add(ItemID item, amount_t amount)
{
	if (!IsIDInBounds(item)) return;
	
	// Doesn't do anything if the resulting amount of ItemID is greater than the max stack size of 65535
	if (GetAmount(item) + amount > 0xffff) return; 

	m_inventoryItems[item] += amount;
}
void Inventory::Remove(ItemID item, amount_t amount)
{
	if (!IsIDInBounds(item)) return;

	if (GetAmount(item) < amount) return;

	m_inventoryItems[item] -= amount;
}

void Inventory::ShowContents()
{
	printf("Content of Inventory:\n");

	int itemAmount = ItemRegistry::Items.size();

	for (int itemID = 0; itemID < itemAmount; itemID++)
	{
		printf(" - ID: %d | Item: %s | Amount: %d\n", itemID, ItemRegistry::Items[itemID]->name.c_str(), m_inventoryItems[itemID]);
	}
}

