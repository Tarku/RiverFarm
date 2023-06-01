#include "Entity.h"

PlayerEntity::PlayerEntity(const v2f& _position)
{
	position = _position;

	atlasID = { 0, 0 };

	Entity::Entities.push_back(this);
}

void PlayerEntity::Draw(sf::RenderWindow* window)
{
	
}