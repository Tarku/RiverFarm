#include "ToolDeclarations.h"

HoeTool::HoeTool()
{
	name = "Hoe";
	uiIcon = { 2, 0 };
}

bool HoeTool::CanBeUsedHere(World* world, const Vector2f& position)
{
	return (world->TileAt(position, 0) == TileID::Dirt || world->TileAt(position, 0) == TileID::Grass) && world->IsEmptyAt(position, 1);
}

void HoeTool::OnUse(World* world, const Vector2f& position)
{
	world->SetTile(position, 0, TileID::TilledSoil);
}