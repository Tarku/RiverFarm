#include "ToolDeclarations.h"

ScytheTool::ScytheTool()
{
	name = "Scythe";
	description = "Is used to harvest plants.";

	uiIcon = { 0, 1 };
}

bool ScytheTool::CanBeUsedHere(World* world, const sf::Vector2f& position)
{
	return world->TileAt(position, 1) == TileID::Flowers;
}

void ScytheTool::OnUse(World* world, const sf::Vector2f& position)
{
	TileRegistry::Tiles[world->TileAt(position, 1)].OnTileBreak(position, world, 1);
	world->SetTile(position, 1, TileID::Air);
}