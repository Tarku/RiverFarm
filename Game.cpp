#include "Game.h"
#include <iostream>

using namespace sf;

PlayerEntity Game::player = PlayerEntity(v2f(MAP_WIDTH / 2, MAP_HEIGHT / 2));
bool Game::showChunkBorders = false;

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
	Inventory::Initialize();

	Interface::CreateNormalizedText(m_currentTool->name, v2f(0.5f, 0.075f));
	Interface::CreateNormalizedText(std::format("FPS: {}", (int)m_fps), v2f(0.0f, 0.00f), sf::Color::Yellow, 1.0F, false);
	Interface::CreateNormalizedText(std::format("Camera X, Y: {}, {}", m_cameraPosition.x, m_cameraPosition.y), v2f(0.f, 0.1f), sf::Color::White, 1.0F, false);
}

v2f Game::ScreenToWorld(const v2f& position)
{
	v2f output = position + m_cameraPosition;

	output.x /= (float)TILE_SIZE * TEXTURE_SCALE;
	output.y /= (float)TILE_SIZE * TEXTURE_SCALE;

	return output;
}

void Game::HandleEvents()
{
	while (m_window.pollEvent(m_curEvent))
	{	
		Vector2i m = Mouse::getPosition(m_window);

		m_mousePosition = v2f( (float) m.x, (float) m.y ) ;

		if (m_curEvent.type == Event::Closed)
			m_window.close();

		if (m_curEvent.type == Event::KeyPressed)
		{
			if (m_curEvent.key.code == Keyboard::R)
				m_world.DoWorldGen();
			if (m_curEvent.key.code == Keyboard::I)
			{
				Inventory::ShowContents();
			}
			if (m_curEvent.key.code == Keyboard::G)
			{
				Game::showChunkBorders = !Game::showChunkBorders;
			}

		}

		if (m_curEvent.type == Event::MouseWheelScrolled)
		{
			//Utils::Log(std::format("{}", m_curEvent.mouseWheelScroll.delta));

			if (m_curEvent.mouseWheelScroll.delta < 0)
				m_currentToolIndex--;

			if (m_curEvent.mouseWheelScroll.delta > 0)
				m_currentToolIndex++;

			if (m_currentToolIndex < 0)
				m_currentToolIndex = ToolRegistry::ToolCount() - 1;

			if (m_currentToolIndex >= ToolRegistry::ToolCount())
				m_currentToolIndex = 0;
		}

		if (m_curEvent.type == Event::MouseButtonPressed)
		{
			if (m_curEvent.mouseButton.button == Mouse::Left)
			{

				v2f mousePosition = ScreenToWorld(m_mousePosition);

				if (m_world.InBounds(mousePosition, 0))
				{
					if (m_currentTool->CanBeUsedHere(&m_world, mousePosition))
						m_currentTool->OnUse(&m_world, mousePosition);
				}
			}
		}


		player.HandleEvents(&m_curEvent);

		for (auto& entity : World::WorldEntities)
		{
			entity->HandleEvents(&m_curEvent);
		}

	}
}

void Game::Update(float timeElapsed)
{
	HandleEvents();

	m_currentTool = ToolRegistry::Tools[m_currentToolIndex];

	player.Update(&m_world, timeElapsed);

	for (auto entity : World::WorldEntities)
	{
		entity->Update(&m_world, timeElapsed);
	}
	for (auto entity : World::EntitiesToDelete)
	{
		m_world.RemoveEntity(entity);

	}
	
	m_world.Update(m_cameraPosition, timeElapsed);


	m_fps = 1.f / timeElapsed;

	Interface::SetTextString(0, m_currentTool->name);
	Interface::SetTextString(1, std::format("FPS: {}", (int)m_fps));
	Interface::SetTextString(2, std::format("Player X, Y: {}, {}", (int) player.position.x, (int)player.position.y));

	m_gameTime += timeElapsed;
	m_ticks++;
}

void Game::Draw()
{
	m_window.clear();

	m_cameraPosition = player.position * (float) SCALED_TILE_SIZE - v2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	int chunksRendered = m_world.DrawChunks(&m_window, m_cameraPosition);

	m_window.setTitle(std::format("River Farm - {} chunks rendered", chunksRendered));

	player.Draw(&m_window, m_cameraPosition);

	for (auto& entity : World::WorldEntities)
	{
		entity->Draw(&m_window, m_cameraPosition);
	}

	Interface::DrawUIElementNormalized(m_currentTool->uiIcon, v2f(0.5f, 0));

	Interface::DrawText(0);
	Interface::DrawText(1);
	Interface::DrawText(2);

	Interface::ShowInventoryOverlay();

	m_window.display();

}

void Game::Dispose()
{
	//delete m_currentTool;
	//delete[] &m_tools;
	Interface::Dispose();
	m_world.Dispose();

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