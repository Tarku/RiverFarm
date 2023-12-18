#include "ToolDeclarations.h"
#include "../World/Tiles/TileRegistry.h"

ShovelTool::ShovelTool()
{
	name = "Shovel"; 
	uiIcon = AtlasID( 3, 0 );
	soundEffectTag = "shovel";
}

bool ShovelTool::CanBeUsedHere(World* world, const sf::Vector2f& position)
{
	return TileRegistry::Tiles[world->TileAt(position, 0)]->tileProperties.isDiggable && world->IsEmptyAt(position, 1);
}

void ShovelTool::OnUse(World* world, const sf::Vector2f& position)
{
	Metadata metaAt = world->MetaAt(position, 0);

	uint8_t randDamage = 2;

	if ((uint8_t)metaAt.damage + randDamage >= 0xF)
		metaAt.damage = 0xF;
	else
		metaAt.damage += randDamage;

	world->SetMeta(position, 0, metaAt);
	TileRegistry::Tiles[world->TileAt(position, 0)]->TryTileBreak(position, world, 0);
}
