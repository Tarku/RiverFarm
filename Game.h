#pragma once

#include <SFML/Window.hpp>
#include <time.h>

#include "EntityDeclarations.h"
#include "TileRegistry.h"
#include "ToolDeclarations.h"
#include "Shared.h"
#include "Interface.h"
#include "World.h"
#include "Inventory.h"
#include "AtlasManager.h"
#include "Utils.h"

using namespace sf;

class Game
{
private:
	short m_currentToolIndex = 0;
	AgriculturalTool* m_currentTool = m_tools[m_currentToolIndex];

	v2f m_cameraPosition = v2f(0, 0);
	v2f m_mousePosition;

	Interface m_interface;

	Image m_iconImage;

	PlayerEntity m_player = PlayerEntity(v2f(0, 0));
	

	Clock m_clock;

	float m_fps = 0;

	long m_ticks = 0;

	RenderWindow m_window = RenderWindow(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "River Farm", Style::Titlebar | Style::Close);
	Event m_curEvent;

	World m_world;
public:
	static time_t Seed;
	static AgriculturalTool* m_tools[15];

	Game();


	v2f ScreenToWorld(v2f& position);

	void HandleEvents();
	void Update(float timeElapsed);
	void Draw();
	void Dispose();
	void Run();
};

