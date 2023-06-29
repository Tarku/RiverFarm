#include "Game.h"
#include <iostream>

using namespace sf;

AgriculturalTool* Game::m_tools[] = {
	new AxeTool(),
	new HoeTool(),
	new ShovelTool()
};

time_t Game::Seed = time((time_t*)0);

Game::Game()
{

	bool atlasesLoaded = AtlasManager::LoadAtlases();

	if (!atlasesLoaded) printf("Couldn't load atlases!\n");

	m_iconImage.loadFromFile("icon.png");

	m_window.setFramerateLimit(60);
	m_window.setIcon(m_iconImage.getSize().x, m_iconImage.getSize().y, m_iconImage.getPixelsPtr());
	
	m_textureAtlas.loadFromFile("atlas.png");
	m_font.loadFromFile("font.ttf");

	Inventory::Initialize();

	m_uiView = m_window.getDefaultView();
	m_spriteView = View(v2f(0, 0), v2f(WINDOW_WIDTH, WINDOW_HEIGHT));

	Interface::Initialize(&m_window);

	m_toolText = Text(m_currentTool->name, m_font);
	
	printf("Size of Sprite: %d bytes.\n", (int) sizeof(Sprite));

	const int totalMapSprites = MAP_HEIGHT* MAP_WIDTH* MAP_LEVELS;

	printf("Size of %dx%dx%d sprites: %d bytes.\n", MAP_HEIGHT, MAP_WIDTH, MAP_LEVELS, (int) (totalMapSprites * sizeof(Sprite)));

	srand(Game::Seed);

	Interface::CreateNormalizedText(m_currentTool->name, Vector2f(0.5f, 0.075f));
	Interface::CreateNormalizedText(std::format("FPS: {}", (int)m_fps), Vector2f(0.0f, 0.00f), sf::Color::Yellow, 1.0F, false);
	Interface::CreateNormalizedText(std::format("Camera X, Y: {}, {}", m_cameraPosition.x, m_cameraPosition.y), Vector2f(0.f, 0.1f), sf::Color::White, 1.0F, false);
}

Vector2f Game::ScreenToWorld(Vector2i& position)
{
	Vector2f output = m_window.mapPixelToCoords(position, m_spriteView);

	output.x /= (float)TILE_SIZE * TEXTURE_SCALE;
	output.y /= (float)TILE_SIZE * TEXTURE_SCALE;

	return output;
}

void Game::HandleEvents()
{
	while (m_window.pollEvent(m_curEvent))
	{	
		m_mousePosition = Mouse::getPosition(m_window);


		for (auto& entity : World::WorldEntities)
		{
			entity->HandleEvents(&m_curEvent);
		}

		if (m_curEvent.type == Event::Closed) m_window.close();
		if (m_curEvent.type == Event::KeyPressed)
		{
			if (m_curEvent.key.code == Keyboard::R)
				m_world.DoWorldGen();

			if (m_curEvent.key.code == Keyboard::I)
				Inventory::ShowContents();

			if (m_curEvent.key.code == Keyboard::Left)
				m_currentToolIndex--;

			if (m_curEvent.key.code == Keyboard::Right)
				m_currentToolIndex++;

			if (m_currentToolIndex < 0)
				m_currentToolIndex = sizeof(m_tools) / sizeof(AgriculturalTool);

			if (m_currentToolIndex > sizeof(m_tools) / sizeof(AgriculturalTool))
				m_currentToolIndex = 0;

			m_currentTool = m_tools[m_currentToolIndex];
		}

		if (m_curEvent.type == Event::MouseButtonPressed)
		{
			if (m_curEvent.mouseButton.button == Mouse::Left)
			{

				Vector2f mousePosition = ScreenToWorld(m_mousePosition);

				if (m_world.InBounds(mousePosition, 0))
				{
					if (m_currentTool->CanBeUsedHere(&m_world, mousePosition))
						m_currentTool->OnUse(&m_world, mousePosition);
				}
			}
		}
	}
}

void Game::Update(float timeElapsed)
{
	HandleEvents();

	int i = 0;
	for (auto& entity : World::WorldEntities)
	{
		entity->Update(&m_world, timeElapsed);
		i++;
	}

	m_ticks++;

	if (m_ticks % 20 == 0)
		m_world.Update();

	m_fps = 1.f / timeElapsed;

	Interface::SetTextString(0, m_currentTool->name);
	Interface::SetTextString(1, std::format("FPS: {}", (int)m_fps));
	Interface::SetTextString(2, std::format("Player X, Y: {}, {}", (int) m_player.position.x / TILE_SIZE, (int)m_player.position.y / TILE_SIZE));
}

void Game::Draw()
{
	m_window.clear();

	m_cameraPosition = m_player.position + v2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	m_window.setView(m_spriteView);

	m_world.DrawChunks(&m_window, m_cameraPosition);

	for (auto& entity : World::WorldEntities)
	{
		entity->Draw(&m_window);
	}

	m_window.setView(m_uiView);

	Interface::DrawUIElementNormalized(m_currentTool->uiIcon, Vector2f(0.5f, 0));

	Interface::DrawText(0);
	Interface::DrawText(1);
	Interface::DrawText(2);

	m_window.display();

}

void Game::Run()
{
	while (m_window.isOpen())
	{
		sf::Time elapsed = m_clock.restart();
		Update(elapsed.asSeconds());
		Draw();

	}

	Interface::Dispose();
}