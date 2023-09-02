#include "ToolDeclarations.h"

MilkPailTool::MilkPailTool()
{
	name = "Milk Pail";
	description = "Is used to milk cows or pick up water.";
	uiIcon = AtlasID(2, 1);
}

bool MilkPailTool::CanBeUsedHere(World* world, const v2f& position)
{
	for (auto entity : World::WorldEntities)
	{
		auto cowCast = dynamic_cast<CowEntity*>(entity);

		if (cowCast == 0)
		{
			continue;
		}

		if (cowCast->position.x >= position.x && cowCast->position.y > position.y && cowCast->position.x < position.x + 1.f && cowCast->position.y < position.y + 1.f && cowCast->milkFullness >= 1)
		{
			m_lastPailAction = ActionMilkCow;
			m_lastCowMilked = cowCast;
			return true;
		}
	}

	if (world->TileAt(position, 0) == TileID::Water)
	{

		m_lastPailAction = ActionTakeWater;
		return true;
	}

	return false;
}

void MilkPailTool::OnUse(World* world, const v2f& position)
{
	switch (m_lastPailAction)
	{
	case ActionMilkCow:
		if (m_lastCowMilked != nullptr) m_lastCowMilked->milkFullness = 0;
		Inventory::Add(ItemID::ItemMilk, 1);
		break;
	case ActionTakeWater:
		TileRegistry::Tiles[world->TileAt(position, 0)]->OnTileBreak(position, world, 0);
		Inventory::Add(ItemID::ItemWater, 1);
		break;

	}
}