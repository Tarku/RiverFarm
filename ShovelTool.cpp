#include "ToolDeclarations.h"
#include "TileRegistry.h"

ShovelTool::ShovelTool()
{
	name = "Shovel"; 
	uiIcon = { 3, 0 };
}

bool ShovelTool::CanBeUsedHere(World* world, const sf::Vector2f& position)
{
	return TileRegistry::Tiles[world->TileAt(position, 0)].tileCategory == TileCategory::DirtLike && world->IsEmptyAt(position, 1);
}

void ShovelTool::OnUse(World* world, const sf::Vector2f& position)
{
	TileRegistry::Tiles[world->TileAt(position, 0)].OnTileBreak(position, world);
}
