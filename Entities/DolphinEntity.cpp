#include "DolphinEntity.h"

DolphinEntity::DolphinEntity(const v2f& position)
{
	this->position = position;

	float startDirection = Utils::RandInt(0, 360) * RADIAN;

	this->velocity.x = cosf(startDirection);
	this->velocity.y = sinf(startDirection);
	

	atlasID = { 5, 0 };
	speed = 1.4f;
}

void DolphinEntity::HandleEvents(sf::Event* event)
{

}
void DolphinEntity::Update(World* world, float dt)
{
	position += velocity * speed * dt;
}

void DolphinEntity::Draw(sf::RenderWindow* window, v2f cameraPosition)
{

	MovingEntity::Draw(window, cameraPosition);
}

void DolphinEntity::RandomizeDirection()
{

}