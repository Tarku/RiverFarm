#include "EntityDeclarations.h"

PlayerEntity::PlayerEntity(const v2f& _position)
{
	position = _position;

	atlasID = { 0, 0 };

	Entity::Entities.push_back(this);
}

void PlayerEntity::HandleEvents(Event* event)
{
	switch (event->type)
	{
	case sf::Event::KeyPressed:
		switch (event->key.code)
		{
		case Keyboard::Z:
			velocity -= v2f(0, 1);
			break;
		case Keyboard::S:
			velocity += v2f(0, 1);
			break;
		case Keyboard::Q:
			velocity -= v2f(1, 0);
			break;
		case Keyboard::D:
			velocity += v2f(1, 0);
			break;
		}
	}
}

void PlayerEntity::Update(float dt)
{
	position += velocity * dt * m_speed;

	velocity *= 0.75f;
}

void PlayerEntity::Draw(sf::RenderWindow* window)
{
	Texture* t = AtlasManager::GetAtlas(AtlasTextureID::Entities);
	Sprite s = Sprite(*t, IntRect(atlasID.x * TILE_SIZE, atlasID.y * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	s.setPosition(position);
	s.setScale(TEXTURE_SCALE, TEXTURE_SCALE);

	window->draw(s);
}