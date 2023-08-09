#include "SceneDeclarations.h"
#include "SceneManager.h"
#include "../Tools/ToolRegistry.h"

using namespace sf;


PlayerEntity GameScene::player = PlayerEntity(v2f(MAP_WIDTH / 2, MAP_HEIGHT / 2));
bool GameScene::showChunkBorders = false;

void GameScene::Initialize(RenderWindow* window)
{
	p_window = window;

	p_interface = Interface();

	Inventory::Initialize();

	p_interface.CreateNormalizedText(std::string("ui_tool_name_text"), "", v2f(0.5f, 0.075f));
	p_interface.CreateNormalizedText(std::string("fps_text"), "", v2f(0.0f, 0.00f), sf::Color::Yellow, 1.0F, false);
	p_interface.CreateNormalizedText(std::string("position_text"), "", v2f(0.f, 0.1f), sf::Color::White, 1.0F, false);

	Utils::Log("World created.");

	m_world.DoWorldGen();
}

v2f GameScene::ScreenToWorld(const v2f& position)
{
	v2f output = position + m_cameraPosition;

	output.x /= (float)TILE_SIZE * TEXTURE_SCALE;
	output.y /= (float)TILE_SIZE * TEXTURE_SCALE;

	return output;
}

void GameScene::HandleEvents()
{
	while (p_window->pollEvent(p_event))
	{
		Vector2i m = Mouse::getPosition(*p_window);

		m_mousePosition = v2f((float)m.x, (float)m.y);

		if (p_event.type == Event::Closed)
			p_window->close();

		if (p_event.type == Event::KeyPressed)
		{
			switch (p_event.key.code)
			{
			case Keyboard::R:
				m_world.DoWorldGen();
				break;
			case Keyboard::G:
				GameScene::showChunkBorders = !GameScene::showChunkBorders;
				break;
			case Keyboard::Escape:
				SceneManager::ChangeScene(SceneManager::mainMenuScene);
				break;
			}
		}

		if (p_event.type == Event::MouseWheelScrolled)
		{
			if (p_event.mouseWheelScroll.delta < 0)
				m_currentToolIndex--;

			if (p_event.mouseWheelScroll.delta > 0)
				m_currentToolIndex++;

			if (m_currentToolIndex < 0)
				m_currentToolIndex = ToolRegistry::ToolCount() - 1;

			if (m_currentToolIndex >= ToolRegistry::ToolCount())
				m_currentToolIndex = 0;
		}

		if (p_event.type == Event::MouseButtonPressed)
		{
			if (p_event.mouseButton.button == Mouse::Left)
			{

				v2f mousePosition = ScreenToWorld(m_mousePosition);

				if (m_world.InBounds(mousePosition, 0))
				{
					if (m_currentTool->CanBeUsedHere(&m_world, mousePosition))
						m_currentTool->OnUse(&m_world, mousePosition);
				}
			}
		}


		player.HandleEvents(&p_event);

		for (auto& entity : World::WorldEntities)
		{
			entity->HandleEvents(&p_event);
		}

	}
}

void GameScene::Update(float dt)
{
	HandleEvents();

	m_currentTool = ToolRegistry::Tools[m_currentToolIndex];

	player.Update(&m_world, dt);

	for (auto entity : World::WorldEntities)
	{
		entity->Update(&m_world, dt);
	}
	for (auto entity : World::EntitiesToDelete)
	{
		m_world.RemoveEntity(entity);

	}

	m_world.Update(m_cameraPosition, dt);


	m_fps = 1.f / dt;

	p_interface.SetTextString(std::string("ui_tool_name_text"), m_currentTool->name);
	p_interface.SetTextString(std::string("fps_text"), std::format("FPS: {}", (int)m_fps));
	p_interface.SetTextString(std::string("position_text"), std::format("Player X, Y: {}, {}", (int)player.position.x, (int)player.position.y));

	m_gameTime += dt;
	m_ticks++;
}

void GameScene::Draw()
{


	m_cameraPosition = player.position * (float)SCALED_TILE_SIZE - v2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	int chunksRendered = m_world.DrawChunks(p_window, m_cameraPosition);

	p_window->setTitle(std::format("River Farm - {} chunks rendered", chunksRendered));

	player.Draw(p_window, m_cameraPosition);

	for (auto& entity : World::WorldEntities)
	{
		entity->Draw(p_window, m_cameraPosition);
	}

	if (m_currentTool != nullptr)
		p_interface.DrawUIElementNormalized(m_currentTool->uiIcon, v2f(0.5f, 0));

	p_interface.DrawText(std::string("position_text"));
	p_interface.DrawText(std::string("fps_text"));
	p_interface.DrawText(std::string("ui_tool_name_text"));

	p_interface.ShowInventoryOverlay();

}

void GameScene::Dispose()
{
	p_interface.Dispose();
	m_world.Dispose();

}