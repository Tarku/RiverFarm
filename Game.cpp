#include "Game.h"
#include <iostream>

using namespace sf;

Game::Game()
{
	srand((unsigned int) Game::Seed);

	bool atlasesLoaded = AtlasManager::LoadAtlases();

	if (!atlasesLoaded)
	{
		Utils::Log("Couldn't load atlases!");
	}

	Image iconImage;
	iconImage.loadFromFile("Assets/icon.png");

	m_window.setFramerateLimit(60);
	m_window.setIcon(iconImage.getSize().x, iconImage.getSize().y, iconImage.getPixelsPtr());

	Interface::Initialize(&m_window);
	Utils::Initialize(&m_window);

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
	time_t gameStartTime = Utils::GetTimestamp();
	Utils::Log("Game started.");

	while (m_window.isOpen())
	{
		sf::Time elapsed = m_clock.restart();

		Update(elapsed.asSeconds());
		Draw();
	}

	time_t gameEndTime = Utils::GetTimestamp();

	time_t gameTotalTime = gameEndTime - gameStartTime;

	Utils::Log(
		std::format("Game ended. Game time: {} seconds", static_cast<double>(gameTotalTime) / 1000000.0)
	);

	Dispose();
}