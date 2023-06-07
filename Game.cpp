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
}

Vector2f Game::ScreenToWorld(Vector2i& position)
{
	return { (position.x / TEXTURE_SCALE) / (float) TILE_SIZE, (position.y / TEXTURE_SCALE) / (float) TILE_SIZE };
}

void Game::HandleEvents()
{
	while (m_window.pollEvent(m_curEvent))
	{	
		m_mousePosition = Mouse::getPosition(m_window);


		for (auto& entity : Entity::Entities)
		{
			entity->HandleEvents(&m_curEvent);
		}

		if (m_curEvent.type == Event::Closed) m_window.close();
		if (m_curEvent.type == Event::KeyPressed)
		{
			switch (m_curEvent.key.code)
			{
			case Keyboard::R:
				m_world.DoWorldGen();
				break;
			case Keyboard::I:

				Inventory::ShowContents();
				break;
			case Keyboard::Left:
				m_currentToolIndex--;
				break;
			case Keyboard::Right:
				m_currentToolIndex++;
				break;
			}
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

				Vector2f mousePosition = m_window.mapPixelToCoords(m_mousePosition, m_spriteView);
				/*
				mousePosition += (m_spriteView.getCenter() - m_spriteView.getSize() / 2.f);

				*/
				mousePosition.x /= (float)TILE_SIZE * TEXTURE_SCALE;
				mousePosition.y /= (float)TILE_SIZE * TEXTURE_SCALE;
				printf("%f / %f\n", mousePosition.x, mousePosition.y);


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

	for (auto& entity : Entity::Entities)
	{
		entity->Update(timeElapsed);
	}

	m_ticks++;

	if (m_ticks % 20 == 0)
		m_world.Update();

	m_fps = 1.f / timeElapsed;

	Interface::SetTextString(0, m_currentTool->name);
	Interface::SetTextString(1, std::format("FPS: {}", (int)m_fps));
}

void Game::Draw()
{
	m_window.clear();

	m_spriteView.setCenter(m_player.position + v2f(16, 16));
	m_window.setView(m_spriteView);

	// Tile rendering
	for (int z = 0; z < MAP_LEVELS; z++)
	{
		for (int y = 0; y < MAP_HEIGHT; y++)
		{
			for (int x = 0; x < MAP_WIDTH; x++)
			{
				m_currentTile = TileRegistry::Tiles[m_world.TileAt(Vector2f((float) x, (float) y), z)];
				Sprite s = Sprite(*AtlasManager::GetAtlas(AtlasTextureID::Tiles), IntRect(m_currentTile.id.x * 16, m_currentTile.id.y * 16, 16, 16));

				s.setPosition(Vector2f(x * TEXTURE_SCALE * TILE_SIZE, y * TEXTURE_SCALE * TILE_SIZE));
				s.setScale(TEXTURE_SCALE, TEXTURE_SCALE);

				m_window.draw(s);
			}
		}
	}

	for (auto& entity : Entity::Entities)
	{
		entity->Draw(&m_window);
	}

	m_window.setView(m_uiView);

	Interface::DrawUIElementNormalized(m_currentTool->uiIcon, Vector2f(0.5f, 0));

	Interface::DrawText(0);
	Interface::DrawText(1);

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