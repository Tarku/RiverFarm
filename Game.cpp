#include "Game.h"
#include <iostream>

using namespace sf;

AgriculturalTool* Game::m_tools[] = {
	new AxeTool(),
	new HoeTool(),
	new ShovelTool()
};

long Game::Seed = time((time_t*)0);

Game::Game()
{
	m_iconImage.loadFromFile("icon.png");

	m_window.setFramerateLimit(60);
	m_window.setIcon(m_iconImage.getSize().x, m_iconImage.getSize().y, m_iconImage.getPixelsPtr());
	
	m_textureAtlas.loadFromFile("atlas.png");
	m_font.loadFromFile("font.ttf");

	Inventory::Initialize();

	Interface::Initialize(&m_window);
	EntityManager::Initialize(&m_window);

	Inventory::ShowContents();

	m_toolText = Text(m_currentTool->name, m_font);
	

	srand(Game::Seed);
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

	m_ticks++;

	if (m_ticks % 20 == 0)
		m_world.Update();

	m_fps = 1.f / timeElapsed;
}

void Game::Draw()
{
	m_window.clear();

	// Tile rendering
	for (int z = 0; z < MAP_LEVELS; z++)
	{
		for (int y = 0; y < MAP_HEIGHT; y++)
		{
			for (int x = 0; x < MAP_WIDTH; x++)
			{
				m_currentTile = TileRegistry::Tiles[m_world.TileAt(Vector2f(x, y), z)];
				Sprite s = Sprite(m_textureAtlas, IntRect(m_currentTile.id.x * 16, m_currentTile.id.y * 16, 16, 16));

				s.setPosition(Vector2f(x * TEXTURE_SCALE * TILE_SIZE, y * TEXTURE_SCALE * TILE_SIZE));
				s.setScale(TEXTURE_SCALE, TEXTURE_SCALE);

				m_window.draw(s);
			}
		}
	}


	Interface::DrawUIElementNormalized(m_currentTool->uiIcon, Vector2f(0.5f, 0));
	Interface::DrawTextNormalized(m_currentTool->name, Vector2f(0.5f, 0.075f));

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