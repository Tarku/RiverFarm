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

using namespace sf;

class Game
{
private:
	short m_currentToolIndex = 0;
	AgriculturalTool* m_currentTool = m_tools[m_currentToolIndex];

	Interface m_interface;

	Image m_iconImage;
	Font m_font;

	PlayerEntity m_player = PlayerEntity(v2f(0, 0));

	Text m_toolText;
	

	Clock m_clock;
	float m_lastTick = 0;

	float m_fps = 0;

	Tile m_currentTile;

	long m_ticks = 0;
	VertexArray m_vertices;

	RenderWindow m_window = RenderWindow(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "RiverFarm", Style::Titlebar | Style::Close);
	Event m_curEvent;


	View m_spriteView;
	View m_uiView;

	Texture m_textureAtlas;

	World m_world;

	Vector2i m_mousePosition;
public:
	static time_t Seed;

	Game();

	static AgriculturalTool* m_tools[15];

	Vector2f ScreenToWorld(Vector2i& position);

	void HandleEvents();
	void Update(float timeElapsed);
	void Draw();
	void Run();
};

