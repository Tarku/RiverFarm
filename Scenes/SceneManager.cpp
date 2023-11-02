#include "SceneManager.h"

sf::RenderWindow* SceneManager::m_window;

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