#include "EntityDeclarations.h"
#include "World.h"

PlayerEntity::PlayerEntity(const v2f& _position)
{
	position = _position;

	atlasID = { 0, 0 };


	World::WorldEntities.push_back(this);
	printf("Player was added.\n");
}

void PlayerEntity::HandleEvents(Event* event)
{
	switch (event->type)
	{
	case sf::Event::KeyPressed:
		if (event->key.code == Keyboard::Z)
			velocity.y = -1;

		if (event->key.code == Keyboard::S)
			velocity.y = 1;

		if (event->key.code == Keyboard::Q)
			velocity.x = -1;

		if (event->key.code == Keyboard::D)
			velocity.x = 1;
	}
}

void PlayerEntity::Update(World* world, float dt)
{
	position += velocity * dt * m_speed;
	velocity = v2f(0, 0);
}

void PlayerEntity::Draw(sf::RenderWindow* window)
{

	return;
	Texture* t = AtlasManager::GetAtlas(AtlasTextureID::Entities);
	Sprite s = Sprite(*t, IntRect(atlasID.x * TILE_SIZE, atlasID.y * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	s.setPosition(position - v2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
	s.setScale(TEXTURE_SCALE, TEXTURE_SCALE);

	window->draw(s);
}