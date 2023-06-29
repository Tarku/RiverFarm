#include "ToolDeclarations.h"

AxeTool::AxeTool()
{
	name = "Axe";
	uiIcon = { 1, 0 };
}

bool AxeTool::CanBeUsedHere(World* world, const sf::Vector2f& position)
{
	return (world->TileAt(position, 1) == TileID::Shrub);
}

void AxeTool::OnUse(World* world, const sf::Vector2f& position)
{
	TileRegistry::Tiles[world->TileAt(position, 1)].OnTileBreak(position, world, 1);
}