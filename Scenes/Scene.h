#pragma once

#include "../Shared.h"
#include "../Interface.h"

// Interface for every scene
class IScene
{
protected:
	sf::Event p_event;
	sf::RenderWindow* p_window;
	Interface p_interface;

public:
	inline virtual void Initialize(sf::RenderWindow* window) abstract;

	inline virtual void Draw() abstract;
	inline virtual void Update(float dt) abstract;

	inline virtual void Dispose() abstract;
};

