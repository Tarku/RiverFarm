#include "Game.h"
#include <iostream>

using namespace sf;

time_t Game::Seed = Utils::GetTimestamp();

Game::Game()
{
	srand((unsigned int) Game::Seed);

	bool atlasesLoaded = AtlasManager::LoadAtlases();

	if (!atlasesLoaded) Utils::Log("Couldn't load atlases!");

	m_iconImage.loadFromFile("Assets/icon.png");

	m_window.setFramerateLimit(60);
	m_window.setIcon(m_iconImage.getSize().x, m_iconImage.getSize().y, m_iconImage.getPixelsPtr());

	Interface::Initialize(&m_window);

	SceneManager::Initialize(&m_window);
	SceneManager::ChangeScene(SceneManager::mainMenuScene);
}

void Game::Update(float timeElapsed)
{
	SceneManager::currentScene->Update(timeElapsed);
}

void Game::Draw()
{
	m_window.clear();
	SceneManager::currentScene->Draw();
	m_window.display();

}

void Game::Dispose()
{
	SceneManager::currentScene->Dispose();
	m_window.~RenderWindow();
}

void Game::Run()
{
	while (m_window.isOpen())
	{
		sf::Time elapsed = m_clock.restart();

		Update(elapsed.asSeconds());
		Draw();
	}

	Dispose();
}