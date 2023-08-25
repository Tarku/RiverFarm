#include "Chunk.h"
#include "Tiles/Tile.h"
#include "Tiles/TileRegistry.h"
#include "Crop.h"
#include "World.h"
#include "../Interface.h"

sf::Sprite Chunk::m_tileSprite = Sprite(*AtlasManager::GetAtlas(AtlasTextureID::Tiles), IntRect(0, 0, 16, 16));

Chunk::Chunk(const v2f& position, World* world)
{
	this->position = position;
	this->m_world = world;

	for (int z = 0; z < MAP_LEVELS; z++)
	{
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			for (int x = 0; x < CHUNK_WIDTH; x++)
			{
				m_tiles[z][y][x] = 0;
			}
		}
	}
}
bool Chunk::CanBeRendered(const v2f& cameraPosition)
{
	return
		position.x < (cameraPosition.x + WINDOW_WIDTH) / SCALED_TILE_SIZE
		&& position.y < (cameraPosition.y + WINDOW_HEIGHT) / SCALED_TILE_SIZE
		&& position.x >(cameraPosition.x - WINDOW_WIDTH) / SCALED_TILE_SIZE
		&& position.y >(cameraPosition.y - WINDOW_HEIGHT) / SCALED_TILE_SIZE;
}

void Chunk::Update(float dt)
{
	// Random tick update

	const int blocksToUpdate = 
		(CHUNK_WIDTH * CHUNK_HEIGHT * MAP_LEVELS) / 20;

	for (int i = 0; i < blocksToUpdate; i++)
	{
		int randomX = Utils::RandInt(0, CHUNK_WIDTH - 1);
		int randomY = Utils::RandInt(0, CHUNK_HEIGHT - 1);
		int randomLevel = Utils::RandInt(0, MAP_LEVELS - 1);

		uchar tileID = m_tiles[randomLevel][randomY][randomX];

		v2f worldPosition = m_world->ChunkToWorldPosition(this->position, v2f(randomX, randomY));

		TileRegistry::Tiles[tileID]->OnRandomUpdate(worldPosition, m_world, randomLevel);
	}

	// Crop growth updates

	for (int y = 0; y < CHUNK_HEIGHT; y++)
	{
		for (int x = 0; x < CHUNK_WIDTH; x++)
		{
			if (m_crops[y][x] != nullptr)
				m_crops[y][x]->OnUpdate(v2f(x, y), this, dt);
		}
	}
}

void Chunk::AddCrop(const v2f& inChunkPosition, Crop* crop)
{
	m_crops[(int)inChunkPosition.y][(int)inChunkPosition.x] = crop;
}
void Chunk::RemoveCrop(const v2f& position)
{
	m_crops[(int)position.y][(int)position.x] = nullptr;
}

bool Chunk::IsCrop(const v2f& position)
{
	return m_crops[(int)position.y][(int)position.x] != nullptr;
}
Crop* Chunk::CropAt(const v2f& position)
{
	return m_crops[(int)position.y][(int)position.x];
}


void Chunk::Draw(sf::RenderWindow* window, const v2f& cameraPosition)
{
	for (float z = 0; z < MAP_LEVELS; z++)
	{
		for (float y = 0; y < CHUNK_HEIGHT; y++)
		{
			for (float x = 0; x < CHUNK_WIDTH; x++)
			{
				uchar tileID = TileAt(v2f(x, y), z);

				if (tileID == TileID::Air) // don't waste time drawing air plz
					continue;

				Tile* currentTile = TileRegistry::Tiles[tileID];

				AtlasID tileAtlasId = ((int) z == 0) ? currentTile->groundId : currentTile->textureId;

				m_tileSprite.setTextureRect(IntRect(tileAtlasId.x * TILE_SIZE, tileAtlasId.y * TILE_SIZE, TILE_SIZE, TILE_SIZE));

				m_tileSprite.setPosition(
					v2f(
						(position.x + x) * SCALED_TILE_SIZE,
						(position.y + y) * SCALED_TILE_SIZE
					) - (v2f)cameraPosition
				);

				m_tileSprite.setScale(TEXTURE_SCALE, TEXTURE_SCALE);

				window->draw(m_tileSprite);
			}
		}
	}
	for (int y = 0; y < CHUNK_HEIGHT; y++)
	{
		for (int x = 0; x < CHUNK_WIDTH; x++)
		{
			Crop* currentCrop = m_crops[y][x];

			if (currentCrop == nullptr)
				continue;
				
			AtlasID tileAtlasId = currentCrop->textureID;

			m_tileSprite.setTextureRect(IntRect(tileAtlasId.x * TILE_SIZE, tileAtlasId.y * TILE_SIZE, TILE_SIZE, TILE_SIZE));

			float g = currentCrop->growth;

			m_tileSprite.setPosition(
				v2f(
					(position.x + x + (0.5f * (1 - g))) * SCALED_TILE_SIZE,
					(position.y + y + (0.5f * (1 - g))) * SCALED_TILE_SIZE
				) - (v2f)cameraPosition
			);

			m_tileSprite.setScale(TEXTURE_SCALE * g, TEXTURE_SCALE * g);
			window->draw(m_tileSprite);
		}
	}
}

void Chunk::SetTile(const v2f& position, int layer, uchar tileID)
{
	if (position.x < 0 || position.y < 0 || layer < 0 || position.x >= CHUNK_WIDTH || position.y >= CHUNK_HEIGHT || layer > 1) return;

	m_tiles[layer][(int)position.y][(int)position.x] = tileID;
}

void Chunk::SetTile(const v3f& position, uchar tileID)
{
	if (position.x < 0 || position.y < 0 || position.z < 0 || position.x >= CHUNK_WIDTH || position.y >= CHUNK_HEIGHT || position.z > 1) return;

	m_tiles[(int)position.z][(int)position.y][(int)position.x] = tileID;
}

uchar Chunk::TileAt(const v2f& position, int layer)
{
	if (position.x < 0 || position.y < 0 || layer < 0 || position.x >= CHUNK_WIDTH || position.y >= CHUNK_HEIGHT || layer > 1)
		return -1;

	return m_tiles[layer][(int)position.y][(int)position.x];
}

uchar Chunk::TileAt(const v3f& position)
{
	return uchar();
}

bool Chunk::IsEmptyAt(const v2f& position, int layer)
{
	return false;
}

bool Chunk::IsEmptyAt(const v3f& position)
{
	return false;
}
