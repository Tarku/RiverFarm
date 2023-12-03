#pragma once

#include "../Shared.h"
#include "Scene.h"

class SceneManager
{
private:
	static sf::RenderWindow* m_window;

public:
	static IScene* currentScene;

	static void Initialize(sf::RenderWindow* window);

	static void ChangeScene(IScene* scene);
};