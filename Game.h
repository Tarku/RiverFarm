#pragma once

#include <SFML/Window.hpp>
#include <time.h>

#include "World/TileRegistry.h"
#include "World/World.h"

#include "Entities/EntityDeclarations.h"
#include "Tools/ToolRegistry.h"
#include "Inventory/Inventory.h"
#include "Shared.h"
#include "Interface.h"
#include "AtlasManager.h"
#include "Utils.h"

using namespace sf;

class Game
{
private:
	short m_currentToolIndex = 0;
	AgriculturalTool* m_currentTool = ToolRegistry::Tools[m_currentToolIndex];

	v2f m_cameraPosition = v2f(0, 0);
	v2f m_mousePosition;

	Interface m_interface;

	Image m_iconImage;
	

	Clock m_clock;

	float m_fps = 0;
	float m_gameTime = 0;

	long m_ticks = 0;
	

	RenderWindow m_window = RenderWindow(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "River Farm", Style::Titlebar | Style::Close);
	Event m_curEvent = Event();

	World m_world;
public:

	static PlayerEntity player;
	static time_t Seed;
	static bool showChunkBorders;

	Game();


	v2f ScreenToWorld(const v2f& position);

	void HandleEvents();
	void Update(float timeElapsed);
	void Draw();
	void Dispose();
	void Run();
};

