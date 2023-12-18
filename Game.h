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
			VideoMode::getFullscreenModes()[0],
			"River Farm",
			Style::Fullscreen
		);
	
	void Update(float timeElapsed);
	void Draw();
	void Dispose();

public:
	inline static time_t Seed = Utils::GetTimestamp();
	Game();

	void Run();
};

