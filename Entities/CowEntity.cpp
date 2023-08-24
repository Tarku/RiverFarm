#include "EntityDeclarations.h"
#include "../World/Tiles/TileRegistry.h"

CowEntity::CowEntity(const v2f& position)
{
	this->atlasID = { 1, 0 };
	this->speed = 2.1f;

	OnMoveTimer();

	this->position = position;
}

void CowEntity::Update(World* world, float dt)
{
	MovingEntity::Update(world, dt);
}

void CowEntity::OnMoveTimer()
{
	this->velocity.x = Utils::RandInt(-100, 101) / 100.f;
	this->velocity.y = Utils::RandInt(-100, 101) / 100.f;
}

