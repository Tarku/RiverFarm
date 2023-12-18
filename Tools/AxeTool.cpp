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
	Metadata metaAt = world->MetaAt(position, 1);
	
	uint8_t randDamage = Utils::RandInt(1, 3);

	if ((uint8_t) metaAt.damage + randDamage >= 0xF)
		metaAt.damage = 0xF;
	else
		metaAt.damage += randDamage;

	world->SetMeta(position, 1, metaAt);
	TileRegistry::Tiles[world->TileAt(position, 1)]->TryTileBreak(position, world, 1);
}