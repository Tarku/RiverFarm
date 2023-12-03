#pragma once

#include "EntityDeclarations.h"

class DolphinEntity : public MovingEntity
{
public:
	DolphinEntity(const v2f& position);


	virtual void HandleEvents(Event* event) override;
	virtual void Update(World* world, float dt) override;
	virtual void Draw(sf::RenderWindow* window, v2f cameraPosition) override;

	virtual void RandomizeDirection() override;
};