#include "World.h"
#include "Tile.h"
#include "Game.h"
#include "TileRegistry.h"
#include "Utils.h"

std::vector<Entity*> World::WorldEntities = std::vector<Entity*>();

World::World()
{
	Utils::Log("World created.");

	DoWorldGen();
}

void World::ResetWorld()
{
	for (int y = 0; y < MAP_HEIGHT / CHUNK_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH / CHUNK_WIDTH; x++)
		{
			m_map[y][x] = new Chunk(v2f(x * CHUNK_WIDTH, y * CHUNK_HEIGHT));
		}
	}

}

void World::DoWorldGen()
{
	ResetWorld();

	Game::Seed = Utils::getTimestamp();
	m_perlin.reseed((unsigned int) Game::Seed);

	time_t worldgenStartTime = Utils::getTimestamp();

	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			double perlinValue = m_perlin.octave2D_01(x * (1.f / 8), y * (1.f / 8), 2);
			double perlinValue2 = m_perlin.octave2D_01(x * (1.f / 16), y * (1.f / 16), 1);
			double perlinValue3 = m_perlin.octave2D_01(x * (1.f / 32), y * (1.f / 32), 1);
			double perlinValue4 = m_perlin.octave2D_01(x * (1.f / 64), y * (1.f / 64), 1);

			double erosionValue = m_perlin.octave2D_01(x * (1.f / 8) + x * (1.f / 16), y * (1.f / 16) - y * (1.f / 8), 1);

			perlinValue += perlinValue2;
			perlinValue += perlinValue3;

			perlinValue /= 3;

			perlinValue -= perlinValue4 / 20;


			if (perlinValue < 0.5)
			{
				SetTile(Vector2f(x, y), 0, TileID::Water);
			}
			else if (perlinValue >= 0.5 && perlinValue < 0.56)
			{

				SetTile(Vector2f(x, y), 0, TileID::Sand);
			}
			else
			{
				SetTile(Vector2f(x, y), 0, TileID::Grass);

				
				if (erosionValue > 0.8)
					SetTile(v2f(x, y), 0, TileID::Stone);
				else
				{


					if (rand() % 8 == 0)
						SetTile(Vector2f(x, y), 1, TileID::Shrub);

					if (rand() % 8 == 0)
						SetTile(Vector2f(x, y), 1, TileID::GrassWithFlowers);
				}
			}


		}
	}

	time_t worldgenEndTime = Utils::getTimestamp();
	time_t worldgenTotalTime = worldgenEndTime - worldgenStartTime;

	printf("World generation time: %d microseconds.\n", (int) worldgenTotalTime);
}

void World::AttemptSpreadWater(const v2f& position)
{
	bool cond = TileAt(position, 0) == TileID::Air;

	if (!cond)
		return;

	SetTile(position, 0, TileID::Water);
	// printf("Attempting to spread water at %d;%d\n", position.x, position.y);


	AttemptSpreadWater(Vector2f(position.x + 1, position.y));
	AttemptSpreadWater(Vector2f(position.x - 1, position.y));
	AttemptSpreadWater(Vector2f(position.x, position.y + 1));
	AttemptSpreadWater(Vector2f(position.x, position.y - 1));
}

void World::DrawChunks(RenderWindow* window, const v2f& cameraPosition)
{
	int chunksDrawn = 0;

	for (int y = 0; y < MAP_HEIGHT / CHUNK_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH / CHUNK_WIDTH; x++)
		{
			if (m_map[y][x]->CanBeRendered(cameraPosition))
			{

				m_map[y][x]->Draw(window, cameraPosition);
				chunksDrawn++;
			}
		}
	}

	// printf("%d chunks drawn.\n", chunksDrawn);
}

void World::Update()
{
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
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
					}
				}
			}
		}
	}
}

bool World::InBounds(const v2f& position, int layer)
{
    return 0 <= position.x && position.x < MAP_WIDTH && 0 <= position.y && position.y < MAP_HEIGHT && 0 <= layer && layer < MAP_LEVELS;
}

unsigned char World::TileAt(const v2f& position, int layer)
{
    if (!InBounds(position, layer)) return -1;

	float chunkY = position.y / (float) CHUNK_HEIGHT;
	float chunkX = position.x / (float) CHUNK_WIDTH;

	int chunkPosX = (chunkX - (int)chunkX) * CHUNK_WIDTH;
	int chunkPosY = (chunkY - (int)chunkY) * CHUNK_HEIGHT;

    return m_map[(int) chunkY][(int) chunkX]->TileAt(v2f(chunkPosX, chunkPosY), layer);
}

void World::SetTile(const v2f& position, int layer, unsigned char tileID)
{
    if (!InBounds(position, layer)) return;

	float chunkY = position.y / (float)CHUNK_HEIGHT;
	float chunkX = position.x / (float)CHUNK_WIDTH;


	int chunkPosX = (chunkX - (int)chunkX) * CHUNK_WIDTH;
	int chunkPosY = (chunkY - (int)chunkY) * CHUNK_HEIGHT;


    m_map[(int)chunkY][(int)chunkX]->SetTile(v2f(chunkPosX, chunkPosY), layer, tileID);
}

bool World::IsEmptyAt(const v2f& position, int layer)
{
	if (!InBounds(position, layer)) return false;

	return TileAt(position, layer) == TileID::Air;
}

void World::Dispose()
{
	// Dispose entities

	WorldEntities.clear();

	// Dispose of the map's chunks

	for (int y = 0; y < MAP_HEIGHT / CHUNK_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH / CHUNK_WIDTH; x++)
		{
			delete m_map[y][x];
		}
	}

	Utils::Log("World disposed.");
}