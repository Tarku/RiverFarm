#include "ToolDeclarations.h"
#include "../Interface.h"

void AgriculturalTool::Draw(Interface* inter)
{
	inter->DrawUIElementNormalized(this->uiIcon, v2f(.5f, 0.f), true);
}

AxeTool::AxeTool()
{
	name = "Axe";
	uiIcon = AtlasID( 1, 0 );
	soundEffectTag = "chop";
}

bool AxeTool::CanBeUsedHere(World* world, const sf::Vector2f& position)
{
	return TileRegistry::Tiles[world->TileAt(position, 1)]->tileProperties.isChoppable;
}

void AxeTool::OnUse(World* world, const sf::Vector2f& position)
{
	TileRegistry::Tiles[world->TileAt(position, 1)]->OnTileBreak(position, world, 1);
}