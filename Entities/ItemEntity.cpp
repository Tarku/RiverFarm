#include "EntityDeclarations.h"
#include "../World/World.h"
#include "../Inventory/ItemRegistry.h"
#include "../Game.h"

ItemEntity::ItemEntity(const v2f& _position, ItemID itemID) : m_itemID(itemID)
{
	position = _position;
	printf("ItemEntity (item: %s) was added.\n", ItemRegistry::Items[itemID].name.c_str());
}

void ItemEntity::Update(World* world, float dt)
{
	float dx = World::WorldEntities[0]->position.x - position.x;
	float dy = World::WorldEntities[0]->position.y - position.y;

	float dist_squared = dx * dx + dy * dy;

	float dist_desquared = sqrt(dist_squared);
	printf("Distance squared to player: %f\n", dist_desquared);
	velocity = v2f(dx / dist_desquared, dy / dist_desquared);

	position += velocity * dt * 5.0f;
}

void ItemEntity::Draw(sf::RenderWindow* window)
{
	if (m_itemID == ItemID::ItemAir) return; // Doesn't get drawn if the item is air

	Item i = ItemRegistry::Items[m_itemID];

	Texture* t = AtlasManager::GetAtlas(AtlasTextureID::Items);
	Sprite s = Sprite(*t, IntRect(i.atlasID.x * TILE_SIZE, i.atlasID.y * TILE_SIZE, TILE_SIZE, TILE_SIZE));


	s.setPosition(position * (float) (TILE_SIZE * TEXTURE_SCALE));
	s.setScale(TEXTURE_SCALE, TEXTURE_SCALE);

	window->draw(s);
}

void ItemEntity::HandleEvents(sf::Event* event)
{

}