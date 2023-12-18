#pragma once

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <format>
#include <iostream>
#include <chrono>
#include <memory>
#include <functional>
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

const std::string TEXTURES_PATH = std::string("Assets/Textures/");
const std::string SOUNDS_PATH = std::string("Assets/Sounds/");

constexpr float PI = 3.141592654f;
constexpr float TWO_PI = PI * 2.f;

constexpr float RADIAN = PI / 180.f;


constexpr int SCALED_TILE_SIZE = (int)(TEXTURE_SCALE * TILE_SIZE);

constexpr unsigned int WINDOW_WIDTH = 1920;
constexpr unsigned int WINDOW_HEIGHT = 1080;

constexpr unsigned int FULLSCREEN_WINDOW_WIDTH = 1920;
constexpr unsigned int FULLSCREEN_WINDOW_HEIGHT = 1080;

namespace TimeConstants
{
	constexpr int TICKS_PER_MINUTE = 20;
	constexpr int MINUTES_PER_HOUR = 60;
	constexpr int HOURS_PER_DAY = 24;
	constexpr int DAYS_PER_WEEK = 7;
	constexpr int WEEKS_PER_MONTH = 4;
	constexpr int MONTHS_PER_YEAR = 12;
}

// Just a bunch of shorthands for my lazy brain

enum class Direction
{
	North,
	South,
	West,
	East

};

using v3f = sf::Vector3f;
using v2f = sf::Vector2f;
using v3i = sf::Vector3i;
using v2i = sf::Vector2i;
using v2u = sf::Vector2u;
using uchar = unsigned char;
using uint = unsigned int;

using string = std::string;
using strings = std::vector<string>;

using voidfunc = std::function<void()>;
using voidfuncs = std::vector<voidfunc>;

constexpr v2f ZERO_VEC = v2f(0.f, 0.f);

// All the direct neighbors of a tile, including diagonal ones
const v2f TileNeighbors[8] = {
	v2f(-1, -1), v2f(0, -1),		v2f(1, -1),
	v2f(-1, 0),						v2f(1, 0),
	v2f(-1, 1),  v2f(0, 1),			v2f(1, 1)
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
