#pragma once

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <format>
#include <iostream>
#include <chrono>
#include <memory>
#include <cstdint>

constexpr int MAX_TILES = 256;
constexpr int MAX_ITEMS = 256;
constexpr int MAX_ATLASES = 4;

constexpr int TILE_SIZE = 16;

constexpr int MAP_HEIGHT = 1024;
constexpr int MAP_WIDTH = 1024;

constexpr int CHUNK_HEIGHT = 32;
constexpr int CHUNK_WIDTH = 32;

constexpr int UI_ICON_WIDTH = 32;
constexpr int UI_ICON_HEIGHT = 32;

constexpr int MAP_LEVELS = 2;

constexpr float TEXTURE_SCALE = 2.f;

constexpr unsigned int WINDOW_WIDTH = (TILE_SIZE * CHUNK_WIDTH * 2);
constexpr unsigned int WINDOW_HEIGHT = (TILE_SIZE * CHUNK_HEIGHT * 2);

typedef sf::Vector3f v3f; // shorthand
typedef sf::Vector2f v2f; // shorthand
typedef unsigned char uchar;

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
