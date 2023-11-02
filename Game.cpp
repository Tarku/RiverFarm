#include "Game.h"
#include "Scenes/MainMenuScene.h"
#include "OptionsManager.h"
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
	bool couldLoadIconImage = iconImage.loadFromFile("Assets/icon.png");

	if (!couldLoadIconImage)
	{
		Utils::Log("Couldn't load icon image!");
	}

	Image cursorImage;
	bool couldLoadCursorImage = cursorImage.loadFromFile("Assets/cursor.png");

	if (!couldLoadCursorImage)
	{
		Utils::Log("Couldn't load cursor image!");
	}

	Cursor cursor;
	bool couldInitCursor = cursor.loadFromPixels(cursorImage.getPixelsPtr(), cursorImage.getSize(), Vector2u(0, 0));

	if (!couldInitCursor)
	{
		Utils::Log("Couldn't initialize cursor!");
	}

	m_window.setFramerateLimit(60);

	m_window.setIcon(iconImage);

	m_window.setMouseCursor(cursor);


	Interface::Initialize(&m_window);
	Utils::Initialize(&m_window);

	SceneManager::Initialize(&m_window);
	SceneManager::ChangeScene(new MainMenuScene());
}

void Game::Update(float timeElapsed)
{
	SceneManager::currentScene->Update(timeElapsed);

	if (OptionsManager::currentInputType == OptionsManager::InputType::Controller)
	{
		v2i middle = v2i(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

		int offsetX = (int)OptionsManager::GetJoystickSecondaryAxisX();
		int offsetY = (int)OptionsManager::GetJoystickSecondaryAxisY();

		sf::Mouse::setPosition(middle + v2i(offsetX, offsetY), m_window);
	}
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