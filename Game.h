#pragma once

#include <SFML/Window.hpp>
#include <time.h>


#include "Scenes/SceneManager.h"
#include "Shared.h"
#include "Interface.h"
#include "AtlasManager.h"
#include "Utils.h"

using namespace sf;

class Game
{
private:
	Image m_iconImage;
	Clock m_clock;
	RenderWindow m_window = RenderWindow(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "River Farm", Style::Titlebar | Style::Close);

public:

	static time_t Seed;

	Game();

	void Update(float timeElapsed);
	void Draw();
	void Dispose();
	void Run();
};

