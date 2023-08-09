#include "World.h"
#include "Tile.h"
#include "../Game.h"
#include "TileRegistry.h"
#include "../Utils.h"

std::list<Entity*> World::WorldEntities;
std::list<Entity*> World::EntitiesToDelete;

World::World()
{
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

std::tuple<v2f, v2f> World::WorldToChunkPosition(const v2f& worldPosition)
{

	float chunkY = worldPosition.y / (float)CHUNK_HEIGHT;
	float chunkX = worldPosition.x / (float)CHUNK_WIDTH;

	int chunkPosX = (chunkX - (int)chunkX) * CHUNK_WIDTH;
	int chunkPosY = (chunkY - (int)chunkY) * CHUNK_HEIGHT;

	return std::make_tuple(v2f(chunkX, chunkY), v2f(chunkPosX, chunkPosY));
}

void World::AddDecorations()
{
	for (int i = 0; i < 350; i++)
	{
		int roomX = Utils::RandInt(0, MAP_WIDTH);
		int roomY = Utils::RandInt(0, MAP_HEIGHT);

		int roomWidth = Utils::RandInt(3, 18);
		int roomHeight = Utils::RandInt(5, 14);

		for (int y = 0; y < roomHeight; y++)
		{
			for (int x = 0; x < roomWidth; x++)
			{
				// Set the flooring
				SetTile(v2f(x + roomX, y + roomY), 0, TileID::WoodWall);

				if (x == 0 || y == 0 || x == roomWidth - 1 || y == roomHeight - 1)
				{

					SetTile(v2f(x + roomX, y + roomY), 1, TileID::WoodWall);
				}
				else {

					SetTile(v2f(x + roomX, y + roomY), 1, TileID::Air);
				}

			}
		}

		int doorX = Utils::RandInt(1, roomWidth);
		int doorY = Utils::RandInt(1, roomHeight);

		int randomSide = Utils::RandInt(0, 4);

		switch (randomSide)
		{
		case 0:
			SetTile(v2f(roomX + doorX, roomY + roomHeight - 1), 1, TileID::Air);
			break;
		case 1:
			SetTile(v2f(roomX + roomWidth - 1, roomY + doorY), 1, TileID::Air);
			break;
		case 2:
			SetTile(v2f(roomX, roomY + doorY), 1, TileID::Air);
			break;
		case 3:
			SetTile(v2f(roomX + doorX, roomY), 1, TileID::Air);
			break;
		}
	}
}

void World::DoWorldGen()
{
	ResetWorld();

	Game::Seed = Utils::GetTimestamp();

	m_perlin.reseed((unsigned int) Game::Seed);

	time_t worldgenStartTime = Utils::GetTimestamp();

	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			double perlinValue = m_perlin.octave2D_01(x * (1.f / 8), y * (1.f / 8), 2);
			double perlinValue2 = m_perlin.octave2D_01(x * (1.f / 16), y * (1.f / 16), 1);
			double perlinValue3 = m_perlin.octave2D_01(x * (1.f / 32), y * (1.f / 32), 1);
			double perlinValue4 = m_perlin.octave2D_01(x * (1.f / 64), y * (1.f / 64), 1);

			double erosionValue = m_perlin.octave2D_01(x * (1.f / 8) + x * (1.f / 16), y * (1.f / 16) - y * (1.f / 8), 5);

			perlinValue += perlinValue2;
			perlinValue += perlinValue3;

			perlinValue /= 3;

			perlinValue -= perlinValue4 / 20;


			if (perlinValue < 0.4)
			{
				SetTile(Vector2f(x, y), 0, TileID::Water);
			}
			else if (perlinValue >= 0.4 && perlinValue < 0.46)
			{

				if (erosionValue > 0.7)
					SetTile(v2f(x, y), 0, TileID::Gravel);
				else

					SetTile(Vector2f(x, y), 0, TileID::Sand);
			}
			else
			{
				SetTile(Vector2f(x, y), 0, TileID::Grass);

				if (erosionValue > 0.65 && erosionValue < 0.85)
					SetTile(v2f(x, y), 0, TileID::Dirt);
				else if (erosionValue >= 0.85)
					SetTile(v2f(x, y), 0, TileID::Stone);
				else
				{


					if (Utils::RandInt(0, 8) == 0)
						SetTile(Vector2f(x, y), 1, TileID::Shrub);

					if (Utils::RandInt(0, 8) == 1)
						SetTile(Vector2f(x, y), 1, TileID::Flowers);
				}
			}


		}
	}

	AddDecorations();

	time_t worldgenEndTime = Utils::GetTimestamp();
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

int World::DrawChunks(RenderWindow* window, const v2f& cameraPosition)
{
	int chunksDrawn = 0;
	Vertex vertices[4] = {};

	v2f cameraChunkPosition = v2f((cameraPosition.x / SCALED_TILE_SIZE) / CHUNK_WIDTH, (cameraPosition.y / SCALED_TILE_SIZE) / CHUNK_HEIGHT);

	for (int yOffset = -1; yOffset < 2; yOffset++)
	{
		for (int xOffset = -1; xOffset < 2; xOffset++)
		{
			int chunkY = (int)(cameraChunkPosition.y + yOffset);
			int chunkX = (int)(cameraChunkPosition.x + xOffset);

			if (chunkY < 0 || chunkX < 0 || chunkY >= (MAP_HEIGHT / CHUNK_HEIGHT) || chunkX >= (MAP_WIDTH / CHUNK_WIDTH))
				continue;

			if (m_map[chunkY][chunkX]->CanBeRendered(cameraPosition))
			{

				m_map[chunkY][chunkX]->Draw(window, cameraPosition);
				chunksDrawn++;
			}

			if (GameScene::showChunkBorders)
			{

				vertices[0] = Vertex(v2f(chunkX * SCALED_TILE_SIZE * CHUNK_WIDTH, chunkY * SCALED_TILE_SIZE * CHUNK_WIDTH) - cameraPosition);
				vertices[1] = Vertex(v2f((chunkX + 1) * SCALED_TILE_SIZE * CHUNK_WIDTH, chunkY * SCALED_TILE_SIZE * CHUNK_WIDTH) - cameraPosition);

				window->draw(vertices, 2, sf::Lines);



				vertices[0] = Vertex(v2f(chunkX * SCALED_TILE_SIZE * CHUNK_WIDTH, chunkY * SCALED_TILE_SIZE * CHUNK_WIDTH) - cameraPosition);
				vertices[1] = Vertex(v2f(chunkX * SCALED_TILE_SIZE * CHUNK_WIDTH, (chunkY + 1) * SCALED_TILE_SIZE * CHUNK_WIDTH) - cameraPosition);

				window->draw(vertices, 2, sf::Lines);
			}
		}
	}

	return chunksDrawn;
	// printf("%d chunks drawn.\n", chunksDrawn);
}

void World::Update(const v2f& cameraPosition, float dt)
{

	v2f cameraChunkPosition = v2f((cameraPosition.x / SCALED_TILE_SIZE) / CHUNK_WIDTH, (cameraPosition.y / SCALED_TILE_SIZE) / CHUNK_HEIGHT);

	for (int yOffset = 0; yOffset < 1; yOffset++)
	{
		for (int xOffset = 0; xOffset < 1; xOffset++)
		{
			int chunkY = (int)(cameraChunkPosition.y + yOffset);
			int chunkX = (int)(cameraChunkPosition.x + xOffset);

			if (chunkY < 0 || chunkX < 0 || chunkY >= (MAP_HEIGHT / CHUNK_HEIGHT) || chunkX >= (MAP_WIDTH / CHUNK_WIDTH))
				continue;

			m_map[chunkY][chunkX]->Update(dt);

			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				for (int x = 0; x < CHUNK_WIDTH; x++)
				{
					v2f positionInChunk = Vector2f(chunkX * CHUNK_WIDTH + x, chunkY * CHUNK_HEIGHT + y);
					//m_map[y][x]->Update(dt);
					/*
					if (TileAt(positionInChunk, 0) == TileID::TilledSoil)
						for (auto neighbor : neighbors)
						{
							if (TileAt(positionInChunk + neighbor, 0) == TileID::Water)
							{
								SetTile(positionInChunk, 0, TileID::WateredTilledSoil);
								break;
							}
						}

					if (TileAt(positionInChunk, 0) == TileID::Air)
					{

						for (auto neighbor : neighbors)
						{
							if (TileAt(positionInChunk + neighbor, 0) == TileID::Water)
							{
								SetTile(positionInChunk, 0, TileID::Water);
							}
						}
					}*/
				}
			}
		}
	}
	
}

Chunk* World::GetChunk(const v2f& position)
{
	return m_map[(int) position.y][(int) position.x];
}

bool World::InBounds(const v2f& position, int layer)
{
    return 0 <= position.x && position.x < MAP_WIDTH && 0 <= position.y && position.y < MAP_HEIGHT && 0 <= layer && layer < MAP_LEVELS;
}

unsigned char World::TileAt(const v2f& position, int layer)
{
    if (!InBounds(position, layer)) return -1;

	auto chunkPosition = std::get<0>(WorldToChunkPosition(position));
	auto chunkPositionOffset = std::get<1>(WorldToChunkPosition(position));

    return m_map[(int)chunkPosition.y][(int)chunkPosition.x]->TileAt(chunkPositionOffset, layer);
}

unsigned char World::TileAt(int x, int y, int layer)
{
	if (!InBounds(Vector2f(x, y), layer)) return -1;

	auto chunkPosition = std::get<0>(WorldToChunkPosition(v2f(x, y)));
	auto chunkPositionOffset = std::get<1>(WorldToChunkPosition(v2f(x, y)));

	return m_map[(int)chunkPosition.y][(int)chunkPosition.x]->TileAt(chunkPositionOffset, layer);
}

void World::SetTile(const v2f& position, int layer, unsigned char tileID)
{
    if (!InBounds(position, layer)) return;

	auto chunkPosition = std::get<0>(WorldToChunkPosition(position));
	auto chunkPositionOffset = std::get<1>(WorldToChunkPosition(position));


    m_map[(int)chunkPosition.y][(int)chunkPosition.x]->SetTile(chunkPositionOffset, layer, tileID);
}

void World::AddItemEntity(const v2f& position, ItemID itemID, int amount)
{
	for (int i = 0; i < amount; i++)
	{

		ItemEntity* newEntity = new ItemEntity(position, itemID);

		WorldEntities.push_back(newEntity);

	}
}

void World::RemoveEntity(Entity* entity)
{
	if (World::WorldEntities.size() != 0)
	{
		World::WorldEntities.remove(entity);
	}
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

	Utils::Log("World disposed.");
}