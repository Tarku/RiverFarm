#include "ToolDeclarations.h"

HoeTool::HoeTool()
{
	name = "Hoe";
	uiIcon = AtlasID( 2, 0 );
}

bool HoeTool::CanBeUsedHere(World* world, const Vector2f& position)
{
	return TileRegistry::Tiles[world->TileAt(position, 0)]->tileProperties.isArable && world->IsEmptyAt(position, 1);
}

void HoeTool::OnUse(World* world, const Vector2f& position)
{
	world->SetTile(position, 0, TileID::TilledSoil);
}