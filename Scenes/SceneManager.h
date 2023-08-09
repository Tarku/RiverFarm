#pragma once

#include "../Shared.h"
#include "Scene.h"
#include "SceneDeclarations.h"

class SceneManager
{
private:
	static sf::RenderWindow* m_window;

public:
	static MainMenuScene* mainMenuScene;
	static GameScene* gameScene;

	static IScene* currentScene;

	static void Initialize(sf::RenderWindow* window);

	static void ChangeScene(IScene* scene);
};