#include "GameScene.h"
#include "MainMenuScene.h"
#include "SceneManager.h"
#include "../OptionsManager.h"
#include "../SoundManager.h"
#include "../Tools/ToolRegistry.h"
#include "../World/WorldGen.h"
#include "../World/BiomeRegistry.h"
#include "../Entities/DolphinEntity.h"
#include "../World/Tiles/BuildableTileRegistry.h"

using namespace sf;

void GameScene::Initialize(RenderWindow* window)
{
	p_window = window;

	p_interface = Interface();

	Player = new PlayerEntity(v2f(MAP_WIDTH / 2, MAP_HEIGHT / 2));

	bool couldLoadDNCOverlay = m_daynightCycleOverlay.loadFromFile(TEXTURES_PATH + "daynight_overlay.png");

	if (!couldLoadDNCOverlay)
	{
		Utils::Log("Couldn't load daynight cycle overlay!");
	}

	float fontScreenRatio = 30.f / WINDOW_HEIGHT;

	p_interface.CreateNormalizedText(std::string("ui_tool_name_text"), "", v2f(0.5f, 0.125f), v2f(0.5f, 0.0f));
	p_interface.CreateNormalizedText(std::string("fps_text"), "", v2f(0.05f, 0.05f), ZERO_VEC, sf::Color::Yellow);
	p_interface.CreateNormalizedText(std::string("position_text"), "", v2f(0.05f, 0.15f), ZERO_VEC);
	p_interface.CreateNormalizedText(std::string("current_hour"), "", v2f(0.5f, 0.950f - fontScreenRatio), v2f(0.5f, 0.f));

	p_interface.CreateNormalizedText(std::string("tile_hovered"), "", v2f(0.05f, 0.95f - fontScreenRatio), v2f(0.f, 0.f));
	p_interface.CreateNormalizedText(std::string("biome_debug_info"), "", v2f(0.05f, 0.25f), ZERO_VEC, sf::Color::White, 1.0f);

	Utils::Log("World created.");

	m_world.DoWorldGen();
}

v2f GameScene::ScreenToWorld(const v2f& position)
{
	v2f output = position + CameraPosition;

	output.x /= (float)TILE_SIZE * TEXTURE_SCALE;
	output.y /= (float)TILE_SIZE * TEXTURE_SCALE;

	return output;
}

void GameScene::HandleEvents()
{


	Vector2i mousePositionVec = Mouse::getPosition(*p_window);

	MousePosition = v2f((float)mousePositionVec.x, (float)mousePositionVec.y);
	WorldMousePosition = ScreenToWorld(MousePosition);

	v2f mouseToPlayerVector = WorldMousePosition - Player->position;

	m_isCurrentToolTargetInRange = mouseToPlayerVector.length() <= 3;

	if (m_currentMenuMode == MenuMode::NormalMode)
	{
		HandleNormalModeEvents();
	}
	else {

		HandleBuildingModeEvents();
	}

	Player->HandleEvents(&p_event);

	for (auto& entity : World::WorldEntities)
	{
		entity->HandleEvents(&p_event);
	}

}

void GameScene::HandleNormalModeEvents()
{
	while (p_window->pollEvent(p_event))
	{
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

			case Keyboard::F11:
				OptionsManager::fullscreenMode = !OptionsManager::fullscreenMode;

				break;

			case Keyboard::B:
				m_currentMenuMode = MenuMode::BuildingMode;
				break;

			case Keyboard::O:
				m_world.SaveWorldToImage();
				break;

			case Keyboard::G:
				m_drawChunkBorders = !m_drawChunkBorders;
				break;

			case Keyboard::F3:
				m_drawDebugMenu = !m_drawDebugMenu;

				break;
			case Keyboard::Numpad7:
				Player->noclip = !Player->noclip;
				break;
			case Keyboard::Numpad8:
				Player->speedOffset += 0.5;
				break;
			case Keyboard::Numpad9:
				Player->speedOffset = 0;
				break;



			case Keyboard::P:
				World::WorldEntities.push_back(
					new CowEntity(
						WorldMousePosition
					)
				);
				break;
			case Keyboard::I:
				World::WorldEntities.push_back(
					new DolphinEntity(
						WorldMousePosition
					)
				);
				break;

			case Keyboard::Escape:
				SceneManager::ChangeScene(new MainMenuScene());
				break;
			}
		}

		if (p_event.type == Event::MouseWheelScrolled)
		{
			if (p_event.mouseWheelScroll.delta > 0)
			{
				m_currentToolIndex--;
			}

			if (p_event.mouseWheelScroll.delta < 0)
			{
				m_currentToolIndex++;
			}

			if (m_currentToolIndex < 0)
				m_currentToolIndex = ToolRegistry::ToolCount() - 1;

			if (m_currentToolIndex >= ToolRegistry::ToolCount())
				m_currentToolIndex = 0;
		}


	}
	if (ToolCooldown > 0)
		return;
	
	if (Mouse::isButtonPressed(Mouse::Left))
	{
		if (
			m_world.InBounds(WorldMousePosition, 0)
			&& m_currentTool->CanBeUsedHere(&m_world, WorldMousePosition)
			&& m_isCurrentToolTargetInRange
			&& !Player->inWater
			)
		{
			m_currentTool->OnUse(&m_world, WorldMousePosition);

			if (m_currentTool->soundEffectTag != "")
				SoundManager::PlaySound(m_currentTool->soundEffectTag);

			ToolCooldown = MAX_TOOL_COOLDOWN;
		}
	}
	if (Mouse::isButtonPressed(Mouse::Right))
	{
		if (
			m_world.InBounds(WorldMousePosition, 0)
			&& m_isCurrentToolTargetInRange
			&& !Player->inWater
			)
		{
			m_currentTool->OnRightClick(&m_world, WorldMousePosition);

			ToolCooldown = MAX_TOOL_COOLDOWN;
		}
	}

}

void GameScene::HandleBuildingModeEvents()
{
	while (p_window->pollEvent(p_event))
	{
		
		if (p_event.type == Event::Closed)
		{
			p_window->close();
		}
		if (p_event.type == Event::KeyPressed)
		{
			switch (p_event.key.code)
			{
			case Keyboard::B:
				m_currentMenuMode = MenuMode::NormalMode;
				break;
			case Keyboard::Escape:
				m_currentMenuMode = MenuMode::NormalMode;
				break;
			}
		}

		if (p_event.type == Event::MouseWheelScrolled)
		{
			if (p_event.mouseWheelScroll.delta > 0)
			{
				m_currentBuildableTileIndex--;
			}

			if (p_event.mouseWheelScroll.delta < 0)
			{
				m_currentBuildableTileIndex++;
			}

			if (m_currentBuildableTileIndex < 0)
				m_currentBuildableTileIndex = BuildableTileRegistry::BuildableTileCount() - 1;

			if (m_currentBuildableTileIndex >= BuildableTileRegistry::BuildableTileCount())
				m_currentBuildableTileIndex = 0;
		}

		if (p_event.type == Event::MouseButtonPressed)
		{

			switch (p_event.mouseButton.button)
			{
			case Mouse::Left:
				BuildableTile& bTile = BuildableTileRegistry::BuildableTiles[m_currentBuildableTileIndex];

				if (
					m_world.InBounds(WorldMousePosition, 0)
					&& bTile.CanBePlacedHere(WorldMousePosition, &m_world)
					&& m_isCurrentToolTargetInRange
					&& !Player->inWater
					)
				{
					m_world.SetTile(WorldMousePosition, bTile.tileLayer, bTile.tileID, true);
					m_world.SetMeta(WorldMousePosition, bTile.tileLayer, Metadata(0, 0, 0));


					for (BuildableTileIngredient& bti : bTile.tileIngredients)
					{
						Inventory::Remove(bti.itemID, bti.cost);
					}

					SoundManager::PlaySound("build");
				}
				break;
			}
		}
	}

}

void GameScene::Update(float dt)
{
	HandleEvents();

	if (m_currentMenuMode == MenuMode::NormalMode)
	{
		m_currentTool = ToolRegistry::Tools.at(m_currentToolIndex);
	}


	m_world.Update(CameraPosition, dt);
	Player->Update(&m_world, dt);


	m_fps = 1.f / dt;

	if (m_currentMenuMode == MenuMode::NormalMode)
	{

		p_interface.SetTextString(std::string("ui_tool_name_text"), m_currentTool->name);
		p_interface.SetTextString(std::string("fps_text"), std::format("{} FPS", (int)m_fps));
		p_interface.SetTextString(std::string("position_text"), std::format("X: {}\nY: {}", (int)(Player->position.x * 10) / 10.0f, (int)(Player->position.y * 10) / 10.0f));

		p_interface.SetTextString(std::string("current_hour"), m_world.worldTime.GetDateString());
	}

	m_gameTime += dt;
	ToolCooldown -= dt;
	m_ticks++;
}

void GameScene::DrawTileAtMouseUI()
{
	auto worldChunkCoords = m_world.WorldToChunkPosition(WorldMousePosition);

	v2f chunkPosition = std::get<0>(worldChunkCoords);
	v2f inChunkPositionOffset = std::get<1>(worldChunkCoords);

	Crop* cropAtMousePos = m_world.GetChunk(chunkPosition)->CropAt(inChunkPositionOffset);

	bool isCropAtMousePos = (cropAtMousePos != nullptr);

	std::string cropString = "";

	if (isCropAtMousePos)
	{
		bool isGrowing = cropAtMousePos->isGrowing;
		bool isFullyGrown = cropAtMousePos->isFullyGrown;

		std::string status = "";

		if (isGrowing)
			status = "G";
		else
		{

			status = "NG";

			if (!cropAtMousePos->hasWater)
				status = "NG NW";
			else if (!cropAtMousePos->hasRightSoil)
				status = "NG WS";
		}

		if (isFullyGrown)
			status = "HR";



		cropString = std::format("{} {}% {}", cropAtMousePos->name, roundf(cropAtMousePos->growth * 100), status);
	}

	uchar topTileAtMousePos = m_world.TileAt(WorldMousePosition, 1);

	int layer = 1;

	if (topTileAtMousePos == TileID::Air)
		layer = 0;

	uchar tileHovered = m_world.TileAt(WorldMousePosition, layer);

	Metadata tileMeta = m_world.MetaAt(WorldMousePosition, layer);

	if (tileHovered < 0 || tileHovered >= TileRegistry::TileCount())
	{
		return;
	}

	std::string tileHoveredText;

	if (isCropAtMousePos)
		tileHoveredText = cropString;
	else
		if (m_drawDebugMenu)
		{
			tileHoveredText = std::format("{}({};{};{})", TileRegistry::Tiles[tileHovered]->name, (int)tileMeta.misc, (int)tileMeta.binaryState, (int) tileMeta.damage);
		}
		else
		{
			tileHoveredText = TileRegistry::Tiles[tileHovered]->name;
		}
	p_interface.SetTextString(std::string("tile_hovered"), tileHoveredText);

	p_interface.DrawText(std::string("tile_hovered"));
}

void GameScene::DrawCursorToolUseUI()
{
	// * Draw tool cursor
	AtlasID cursorID = { 0, 0 };
	bool shouldShowCursor = false;

	if (m_currentTool != nullptr)
	{
		cursorID = m_currentTool->uiIcon;
		shouldShowCursor = m_currentTool->CanBeUsedHere(&m_world, WorldMousePosition) && m_isCurrentToolTargetInRange;
	}

	if (shouldShowCursor)
	{
		IntRect cursorImgRect = IntRect(
			v2i(cursorID.x * UI_ICON_WIDTH, cursorID.y * UI_ICON_HEIGHT),
			v2i(UI_ICON_WIDTH, UI_ICON_HEIGHT)
		);

		Sprite toolCursorSprite = Sprite(*AtlasManager::GetAtlas(AtlasTextureID::UI), cursorImgRect);

		v2f cursorPosition = {
			MousePosition.x + 16,
			MousePosition.y + 16
		};

		toolCursorSprite.setPosition(cursorPosition);
		toolCursorSprite.setScale(v2f(1.5f, 1.5f));

		p_window->draw(toolCursorSprite);
	}
}

void GameScene::DrawBiomeDebugInfoUI()
{
	BiomeID biomeAtMouse = WorldGen::GetBiome((int) Player->position.x, (int)Player->position.y);

	double humidityValue = WorldGen::GetHumidityValue((int)Player->position.x, (int)Player->position.y);
	double heatValue = WorldGen::GetHeatValue((int)Player->position.x, (int)Player->position.y);

	p_interface.SetTextString(
		"biome_debug_info",
		std::format(
			"Humidity: {}\nTemperature: {}\nBiome: {}",
			round((1 - humidityValue) * 100) / 100,
			round((1 - heatValue) * 100) / 100,
			BiomeRegistry::Biomes.at(biomeAtMouse)->name
		)
	);
	
	p_interface.DrawText("biome_debug_info");
}

void GameScene::DrawUI()
{
	if (m_currentMenuMode == MenuMode::NormalMode)
	{
		NormalModeDrawUI();
	}
	else {
		BuildingModeDrawUI();
	}

	if (m_world.InBounds(WorldMousePosition, 0))
	{
		DrawTileAtMouseUI();
	}

	//  * Draw inventory screen
	p_interface.ShowInventoryOverlay();



}



void GameScene::BuildingModeDrawUI()
{
	p_interface.ShowBuildableTileOverlay(m_currentBuildableTileIndex);
}
void GameScene::NormalModeDrawUI()
{

	DrawCursorToolUseUI();

	//  * Draw tool overlay 
	p_interface.ShowToolOverlay(m_currentToolIndex);
	p_interface.DrawText(std::string("ui_tool_name_text"));

	//  * Draw text elements
	if (m_drawDebugMenu)
	{
		p_interface.DrawText(std::string("position_text"));
		p_interface.DrawText(std::string("fps_text"));

		DrawBiomeDebugInfoUI();
	}

	p_interface.DrawText(std::string("current_hour"));
}

void GameScene::Draw()
{
	CameraPosition = Player->position * (float)SCALED_TILE_SIZE - v2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	int chunksRendered = m_world.DrawChunks(p_window, CameraPosition, m_drawChunkBorders);

	p_window->setTitle(std::format("River Farm - {} chunks rendered - {} chunks loaded", chunksRendered, m_world.Chunks.size()));

	// -- Draw Player and other entities --

	Player->Draw(p_window, CameraPosition);

	for (auto entity : World::WorldEntities)
	{
		entity->Draw(p_window, CameraPosition);
	}

	//  * Draw daynight overlay

	const int pixelsPerHour = 50;

	int currentHour = static_cast<int>(m_world.worldTime.hours);
	int currentMinute = static_cast<int>(m_world.worldTime.minutes);

	int lightOverlayPixelX = currentHour * pixelsPerHour + (int)((currentMinute / 60.F) * pixelsPerHour);

	//		For smooth transition between the two sides of the image
	if ((uint) lightOverlayPixelX >= m_daynightCycleOverlay.getSize().x)
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

	// -- Draw UI --
	DrawUI();
}

void GameScene::Dispose()
{
	p_interface.Dispose();
	m_world.Dispose();

}