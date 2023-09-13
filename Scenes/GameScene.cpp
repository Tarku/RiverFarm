#include "SceneDeclarations.h"
#include "SceneManager.h"
#include "../Tools/ToolRegistry.h"

using namespace sf;

void GameScene::Initialize(RenderWindow* window)
{
	p_window = window;

	p_interface = Interface();

	player = new PlayerEntity(v2f(MAP_WIDTH / 2, MAP_HEIGHT / 2));

	bool couldLoadDNCOverlay = m_daynightCycleOverlay.loadFromFile("Assets/daynight_overlay.png");

	if (!couldLoadDNCOverlay)
	{
		Utils::Log("Couldn't load daynight cycle overlay!");
	}

	Inventory::Initialize();

	p_interface.CreateNormalizedText(std::string("ui_tool_name_text"), "", v2f(0.5f, 0.075f), v2f(0.5, 0.0f));
	p_interface.CreateNormalizedText(std::string("fps_text"), "", ZERO_VEC, ZERO_VEC, sf::Color::Yellow);
	p_interface.CreateNormalizedText(std::string("position_text"), "", v2f(0.f, 0.1f), ZERO_VEC);
	p_interface.CreateNormalizedText(std::string("current_hour"), "", v2f(0.5f, 0.950f), v2f(0.5f, 0.f));

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
		v2f worldMousePosition = ScreenToWorld(m_mousePosition);

		v2f mouseToPlayerVector = worldMousePosition - player->position;

		m_isCurrentToolTargetInRange = mouseToPlayerVector.length() <= 3;

		if (p_event.type == Event::Closed)
			p_window->close();

		if (p_event.type == Event::KeyPressed)
		{
			switch (p_event.key.code)
			{
			case Keyboard::T:
				AtlasManager::LoadAtlases();
				break;

			case Keyboard::R:
				m_world.DoWorldGen();
				break;

			case Keyboard::G:
				m_drawChunkBorders = !m_drawChunkBorders;
				break;

			case Keyboard::F3:
				m_drawDebugMenu = !m_drawDebugMenu;
				break;

			case Keyboard::P:
				World::WorldEntities.push_back(
					new CowEntity(
						worldMousePosition
					)
				);
				break;

			case Keyboard::Escape:
				SceneManager::ChangeScene(SceneManager::mainMenuScene);
				break;
			}
		}

		if (p_event.type == Event::MouseWheelScrolled)
		{
			if (p_event.mouseWheelScroll.delta < 0)
			{
				m_currentToolIndex--;
			}

			if (p_event.mouseWheelScroll.delta > 0)
			{
				m_currentToolIndex++;
			}

			if (m_currentToolIndex < 0)
				m_currentToolIndex = ToolRegistry::ToolCount() - 1;

			if (m_currentToolIndex >= ToolRegistry::ToolCount())
				m_currentToolIndex = 0;
		}

		if (p_event.type == Event::MouseButtonPressed)
		{
			switch (p_event.mouseButton.button)
			{
			case Mouse::Left:

				if (m_world.InBounds(worldMousePosition, 0) && m_currentTool->CanBeUsedHere(&m_world, worldMousePosition) && m_isCurrentToolTargetInRange)
				{
					m_currentTool->OnUse(&m_world, worldMousePosition);
				}
				break;
			case Mouse::Right:
				if (m_world.InBounds(worldMousePosition, 0) && m_isCurrentToolTargetInRange)
				{
					m_currentTool->OnRightClick(&m_world, worldMousePosition);
				}
				break;
			}
		}

		player->HandleEvents(&p_event);

		for (auto& entity : World::WorldEntities)
		{
			entity->HandleEvents(&p_event);
		}

	}
}

void GameScene::Update(float dt)
{
	HandleEvents();

	m_currentTool = ToolRegistry::Tools.at(m_currentToolIndex);

	player->Update(&m_world, dt);

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
	p_interface.SetTextString(std::string("position_text"), std::format("Player X, Y: {}, {}", (int)player->position.x, (int)player->position.y));

	p_interface.SetTextString(std::string("current_hour"), m_world.worldTime.GetDateString());


	m_gameTime += dt;
	m_toolCooldown += dt;
	m_ticks++;
}

void GameScene::Draw()
{
	m_cameraPosition = player->position * (float)SCALED_TILE_SIZE - v2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	int chunksRendered = m_world.DrawChunks(p_window, m_cameraPosition, m_drawChunkBorders);

	p_window->setTitle(std::format("River Farm - {} chunks rendered", chunksRendered));

	// -- Draw player and other entities --

	player->Draw(p_window, m_cameraPosition);

	for (auto entity : World::WorldEntities)
	{
		entity->Draw(p_window, m_cameraPosition);
	}

	// -- Draw UI --
	//  * Draw daynight overlay

	const int pixelsPerHour = 50;

	int currentHour = m_world.worldTime.hours;
	int currentMinute = m_world.worldTime.minutes;

	int lightOverlayPixelX = currentHour * pixelsPerHour + (int)((currentMinute / 60.F) * pixelsPerHour);

	//		For smooth transition between the two sides of the image
	if (lightOverlayPixelX >= m_daynightCycleOverlay.getSize().x)
	{
		int difference = lightOverlayPixelX - m_daynightCycleOverlay.getSize().x;
		lightOverlayPixelX = difference;
	}

	Sprite daynightOverlaySprite = 
		Sprite(
			m_daynightCycleOverlay,
			IntRect(
				v2i(lightOverlayPixelX, 0),
				v2i(1, 1)
			)
		);

	daynightOverlaySprite.setScale(v2f(WINDOW_WIDTH, WINDOW_HEIGHT));
	daynightOverlaySprite.setPosition(v2f(0, 0));

	p_window->draw(daynightOverlaySprite, RenderStates(sf::BlendMultiply));

	//  * Draw tool overlay
	if (m_currentTool != nullptr)
		m_currentTool->Draw(&p_interface);

	//  * Draw text elements
	if (m_drawDebugMenu)
	{
		p_interface.DrawText(std::string("position_text"));
		p_interface.DrawText(std::string("fps_text"));
	}

	p_interface.DrawText(std::string("ui_tool_name_text"));
	p_interface.DrawText(std::string("current_hour"));

	//  * Draw inventory screen
	p_interface.ShowInventoryOverlay();

	// * Draw tool cursor
	AtlasID cursorID = { 0, 0 };
	bool shouldShowCursor = false;

	v2f worldMousePosition = ScreenToWorld(m_mousePosition);
	
	if (m_currentTool != nullptr)
	{
		cursorID = m_currentTool->uiIcon;
		shouldShowCursor = m_currentTool->CanBeUsedHere(&m_world, worldMousePosition) && m_isCurrentToolTargetInRange;
	}

	if (shouldShowCursor)
	{
		IntRect cursorImgRect = IntRect(
			v2i(cursorID.x * UI_ICON_WIDTH, cursorID.y * UI_ICON_HEIGHT),
			v2i(UI_ICON_WIDTH, UI_ICON_HEIGHT)
		);

		Sprite toolCursorSprite = Sprite(*AtlasManager::GetAtlas(AtlasTextureID::UI), cursorImgRect);

		v2f cursorPosition = {
			m_mousePosition.x + 16,
			m_mousePosition.y + 16
		};

		toolCursorSprite.setPosition(cursorPosition);
		toolCursorSprite.setScale(v2f(1.5f, 1.5f));

		p_window->draw(toolCursorSprite);
	}


}

void GameScene::Dispose()
{
	p_interface.Dispose();
	m_world.Dispose();

}