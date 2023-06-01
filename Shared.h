#pragma once

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <format>
#include <iostream>
#include <chrono>
#include <memory>
#include <cstdint>

#define MAX_TILES (int) 256
#define MAX_ITEMS (int) 256

#define TILE_SIZE (int) 16

#define MAP_HEIGHT (int) 30
#define MAP_WIDTH (int) 30
#define MAP_LEVELS (int) 2

#define TEXTURE_SCALE (int) 2

const unsigned int WINDOW_WIDTH = (TILE_SIZE * TEXTURE_SCALE * MAP_WIDTH);
const unsigned int WINDOW_HEIGHT = (TILE_SIZE * TEXTURE_SCALE * MAP_HEIGHT);

typedef sf::Vector2f v2f;


const sf::Vector2f neighbors[8] = {
		sf::Vector2f(-1, -1), sf::Vector2f(0, -1),  sf::Vector2f(1, -1),
		sf::Vector2f(-1, 0),						sf::Vector2f(1, 0),
		sf::Vector2f(-1, 1), sf::Vector2f(0, 1),    sf::Vector2f(1, 1)
};

// This is pretty much a 2d vector but idc at least it has a more fitting name
struct AtlasID
{
	int x;
	int y;

	bool operator==(const AtlasID& at)
	{
		return x == at.x && y == at.y;
	}
};


