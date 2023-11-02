#include "EntityDeclarations.h"
#include "../Scenes/GameScene.h"
#include "../World/World.h"
#include "../Inventory/ItemID.h"
#include "../Inventory/ItemRegistry.h"
#include "../Game.h"

Texture* Entity::itemsAtlas = AtlasManager::GetAtlas(AtlasTextureID::Items);

ItemEntity::ItemEntity(const v2f& _position, ItemID itemID) : m_itemID(itemID)
{
	m_angle = Utils::RandFloat(0.f, TWO_PI);
	m_scale = 2 * Utils::RandFloat(0.8f, 1.2f);
	position = _position + v2f(Utils::RandFloat(-0.15f, 0.15f), Utils::RandFloat(-0.15f, 0.15f));
	// printf("ItemEntity (item: %s) was added.\n", ItemRegistry::Items[itemID]->name.c_str());

}
ItemEntity::~ItemEntity()
{
	Utils::Log("ItemEntity was deleted");
}

FloatRect ItemEntity::GetRectangle()
{
	return FloatRect(v2f(position.x, position.y), v2f((m_scale - 1), (m_scale - 1)));
}

void ItemEntity::Update(World* world, float dt)
{
	if (m_angle > TWO_PI) m_angle = 0;

	bool hasToBeRemoved = false;

	if (m_lifetime > 0.15f && GameScene::player->GetRectangle().findIntersection(this->GetRectangle()) != std::nullopt)
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
		World::EntitiesToDelete.push_back(this);
	}

	velocity.x = (cos(m_angle) / TILE_SIZE) / 5;
	velocity.y = sin(m_angle) / TILE_SIZE;

	position += velocity * dt * 5.0f;
	m_angle += dt;
	m_lifetime += dt;
}

void ItemEntity::Draw(sf::RenderWindow* window, v2f cameraPosition)
{
	Entity::Draw(window, cameraPosition);

	if (m_itemID == -1) return; // Doesn't get drawn if the item is air

	Item i = *ItemRegistry::Items[m_itemID];

	Sprite s = Sprite(*ItemEntity::itemsAtlas, IntRect(v2i(i.atlasID.x * TILE_SIZE, i.atlasID.y * TILE_SIZE), v2i(TILE_SIZE, TILE_SIZE)));


	s.setPosition(position * static_cast<float> (TILE_SIZE * TEXTURE_SCALE) - cameraPosition);
	s.setScale(v2f(m_scale, m_scale));

	window->draw(s);

	if (p_isHovered)
	{
		Text descriptionText = Text(Interface::font, i.name, 16u);

		descriptionText.setFillColor(sf::Color(255, 255, 255, 255));
		descriptionText.setOutlineColor(sf::Color(0, 0, 0, 255));

		sf::FloatRect textSize = descriptionText.getGlobalBounds();
		descriptionText.setPosition(v2f(position.x * (float)SCALED_TILE_SIZE - cameraPosition.x - textSize.width / 2.f + SCALED_TILE_SIZE / 2, position.y * (float)SCALED_TILE_SIZE - cameraPosition.y - 14));

		window->draw(descriptionText);
	}
}

void ItemEntity::HandleEvents(sf::Event* event)
{

}