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

constexpr int SCALED_TILE_SIZE = (int)(TEXTURE_SCALE * TILE_SIZE);

constexpr unsigned int WINDOW_WIDTH = (int) (TILE_SIZE * CHUNK_WIDTH * 2);
constexpr unsigned int WINDOW_HEIGHT = (int) (TILE_SIZE * CHUNK_HEIGHT * 2);

// Just a bunch of shorthands for my lazy brain

using v3f = sf::Vector3f;
using v2f = sf::Vector2f;
using uchar = unsigned char;
using uint = unsigned int;

const v2f neighbors[8] = {
	v2f(-1, -1), v2f(0, -1),  v2f(1, -1),
	v2f(-1, 0),						v2f(1, 0),
	v2f(-1, 1), v2f(0, 1),    v2f(1, 1)
};

// This is pretty much a 2d vector but idc at least it has a more fitting name

struct AtlasID
{
	int x;
	int y;

	bool operator==(const AtlasID& other)
	{
		return x == other.x && y == other.y;
	}
};
