#pragma once

#include <SFML/Window.hpp>
#include <SFML/Window/Cursor.hpp>
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
	Clock m_clock;
	RenderWindow m_window =
		RenderWindow(
			VideoMode(
				v2u(
					WINDOW_WIDTH,
					WINDOW_HEIGHT
				)
			),
			"River Farm",
			Style::Titlebar | Style::Close
		);
	
	void Update(float timeElapsed);
	void Draw();
	void Dispose();

public:
	inline static time_t Seed = Utils::GetTimestamp();
	Game();

	void Run();
};

