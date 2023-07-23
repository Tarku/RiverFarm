#include "EntityDeclarations.h"
#include "../World/World.h"
#include "../Inventory/ItemRegistry.h"
#include "../Game.h"

Texture* Entity::itemsAtlas = AtlasManager::GetAtlas(AtlasTextureID::Items);

ItemEntity::ItemEntity(const v2f& _position, ItemID itemID) : m_itemID(itemID)
{
	position = _position;
	printf("ItemEntity (item: %s) was added.\n", ItemRegistry::Items[itemID].name.c_str());

}
ItemEntity::~ItemEntity()
{
	Utils::Log("ItemEntity was deleted");
}

void ItemEntity::Update(World* world, float dt)
{
	bool hasToBeRemoved = false;

	if (Game::player.GetRectangle().intersects(this->GetRectangle()))
	{
		Inventory::Add((ItemID) m_itemID, 1);
		hasToBeRemoved = true;
	}

	if (m_lifetime > MAX_ITEM_LIFETIME)
	{
		hasToBeRemoved = true;
	}

	if (hasToBeRemoved)
	{
		world->RemoveEntity(this);
	}
		
	float dx = Game::player.position.x - position.x;
	float dy = Game::player.position.y - position.y;

	float dist_squared = dx * dx + dy * dy;
	float dist = sqrt(dist_squared);

	m_inPlayerRange = dist < 8;

	if (m_inPlayerRange)
		velocity = v2f(dx / dist, dy / dist) / dist;
	else
		velocity = v2f(0.f, 0.f);

	position += velocity * dt * 5.0f;
	m_lifetime += dt;
}

void ItemEntity::Draw(sf::RenderWindow* window, v2f cameraPosition)
{
	if (m_itemID == ItemID::ItemAir) return; // Doesn't get drawn if the item is air

	Item i = ItemRegistry::Items[m_itemID];

	Sprite s = Sprite(*ItemEntity::itemsAtlas, IntRect(i.atlasID.x * TILE_SIZE, i.atlasID.y * TILE_SIZE, TILE_SIZE, TILE_SIZE));


	s.setPosition(position * static_cast<float> (TILE_SIZE * TEXTURE_SCALE) - cameraPosition);
	s.setScale(TEXTURE_SCALE, TEXTURE_SCALE);

	window->draw(s);
}

void ItemEntity::HandleEvents(sf::Event* event)
{

}