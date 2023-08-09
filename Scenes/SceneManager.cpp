#include "SceneManager.h"

sf::RenderWindow* SceneManager::m_window;

MainMenuScene* SceneManager::mainMenuScene = new MainMenuScene();
GameScene* SceneManager::gameScene = new GameScene();

IScene* SceneManager::currentScene;

void SceneManager::Initialize(sf::RenderWindow* window)
{
	m_window = window;
}

void SceneManager::ChangeScene(IScene* scene)
{
	if (SceneManager::currentScene != nullptr)
		SceneManager::currentScene->Dispose();

	SceneManager::currentScene = scene;

	scene->Initialize(m_window);
}