#include "World.h"
#include "Tile.h"
#include "Game.h"
#include "TileRegistry.h"

World::World()
{
	DoWorldGen();
}

void World::ResetWorld()
{

	for (int z = 0; z < MAP_LEVELS; z++)
	{
		for (int y = 0; y < MAP_HEIGHT; y++)
		{
			for (int x = 0; x < MAP_WIDTH; x++)
			{
				m_map[z][y][x] = 0;
			}
		}
	}

}

uint64_t getTimestamp()
{
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
void World::DoWorldGen()
{
	uint64_t t1 = getTimestamp();

	Game::Seed = getTimestamp();
	m_perlin.reseed((unsigned int) Game::Seed);
	ResetWorld();

	int radius = rand() % 5 + 5;

	int midX = MAP_WIDTH / 2;
	int midY = MAP_HEIGHT / 2;

	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			double perlinValue = m_perlin.octave2D_01(x * (1.f / MAP_WIDTH), y * (1.f / MAP_HEIGHT), 1);

			if (perlinValue < 0.4)
			{

				SetTile(Vector2f(x, y), 0, TileID::Water);
			}
			else if (perlinValue >= 0.4 && perlinValue < 0.5)
			{

				SetTile(Vector2f(x, y), 0, TileID::Sand);
			}
			else
			{
				SetTile(Vector2f(x, y), 0, TileID::Grass);
				if (rand() % 8 == 0)
					SetTile(Vector2f(x, y), 1, TileID::Shrub);
			}

		}
	}

	uint64_t t2 = getTimestamp();

	uint64_t dt = t2 - t1;

	printf("World generation time: %d microseconds.\n", (int) dt);
}

void World::AttemptSpreadWater(const Vector2f& position)
{
	bool cond = TileAt(position, 0) == TileID::Air;
	if (!cond)
		return;

	SetTile(position, 0, TileID::Water);
	std::cout << "Attempting to spread water at " << position.x << ";" << position.y << std::endl;


	AttemptSpreadWater(Vector2f(position.x + 1, position.y));
	AttemptSpreadWater(Vector2f(position.x - 1, position.y));
	AttemptSpreadWater(Vector2f(position.x, position.y + 1));
	AttemptSpreadWater(Vector2f(position.x, position.y - 1));
}

void World::Update()
{
	bool stopUpdate = false;
	// std::cout << "World update" << std::endl;
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			if (stopUpdate);

			if (TileAt(Vector2f(x, y), 0) == TileID::Water || x == 0 || y == 0 || x == MAP_WIDTH - 1 || y == MAP_HEIGHT - 1)
			{

				//AttemptSpreadWater(Vector2f(x, y));
			}

			if (TileAt(Vector2f(x, y), 0) == TileID::TilledSoil)
				for (auto neighbor : neighbors)
				{
					if (TileAt(Vector2f(x, y) + neighbor, 0) == TileID::Water)
					{
						SetTile(Vector2f(x, y), 0, TileID::WateredTilledSoil);
						break;
					}
				}

			if (TileAt(Vector2f(x, y), 0) == TileID::Air)
			{

				for (auto neighbor : neighbors)
				{
					if (TileAt(Vector2f(x, y) + neighbor, 0) == TileID::Water)
					{
						SetTile(Vector2f(x, y), 0, TileID::Water);
						stopUpdate = true;
					}
				}
			}
		}
	}
}

bool World::InBounds(const Vector2f& position, int layer)
{
    return 0 <= position.x && position.x < MAP_WIDTH && 0 <= position.y && position.y < MAP_HEIGHT && 0 <= layer && layer < MAP_LEVELS;
}

unsigned char World::TileAt(const sf::Vector2f& position, int layer)
{
    if (!InBounds(position, layer)) return -1;

    return m_map[layer][(int)position.y][(int)position.x];
}

void World::SetTile(const Vector2f& position, int layer, unsigned char tileID)
{
    if (!InBounds(position, layer)) return;

    m_map[layer][(int)position.y][(int)position.x] = tileID;
}

bool World::IsEmptyAt(const sf::Vector2f& position, int layer)
{
	if (!InBounds(position, layer)) return false;
	return m_map[layer][(int)position.y][(int)position.x] == TileID::Air;
}