#include "Chunk.h"
#include "Tile.h"
#include "TileRegistry.h"
#include "Crop.h"
#include "../Interface.h"

sf::Sprite Chunk::m_tileSprite = Sprite(*AtlasManager::GetAtlas(AtlasTextureID::Tiles), IntRect(0, 0, 16, 16));

Chunk::Chunk(const v2f& position)
{
	this->position = position;



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
	for (int y = 0; y < CHUNK_HEIGHT; y++)
	{
		for (int x = 0; x < CHUNK_WIDTH; x++)
		{
			m_crops[y][x] = (((x + y)) > 0) ? new BarleyCrop() : nullptr;
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
	for (int y = 0; y < CHUNK_HEIGHT; y++)
	{
		for (int x = 0; x < CHUNK_WIDTH; x++)
		{
			if (m_crops[y][x] != nullptr)
				m_crops[y][x]->OnUpdate(v2f(x, y), this, dt);
		}
	}
}

void Chunk::AddCrop(const v2f& position, Crop* crop)
{
	m_crops[(int)position.y][(int)position.x] = crop;
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
	for (int z = 0; z < MAP_LEVELS; z++)
	{
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			for (int x = 0; x < CHUNK_WIDTH; x++)
			{
				uchar tileID = TileAt(v2f(x, y), z);

				if (tileID == TileID::Air) // don't draw air plz
					continue;

				Tile currentTile = TileRegistry::Tiles[tileID];
				int scaledTileSize = TILE_SIZE * TEXTURE_SCALE;

				AtlasID tileAtlasId = (z == 0) ? currentTile.groundId : currentTile.textureId;

				m_tileSprite.setTextureRect(IntRect(tileAtlasId.x * TILE_SIZE, tileAtlasId.y * TILE_SIZE, TILE_SIZE, TILE_SIZE));

				m_tileSprite.setPosition(
					v2f(
						(position.x + x) * scaledTileSize,
						(position.y + y) * scaledTileSize
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

			m_tileSprite.setPosition(
				v2f(
					(position.x + x - 1 + currentCrop->growth) * SCALED_TILE_SIZE,
					(position.y + y - 1 + currentCrop->growth) * SCALED_TILE_SIZE
				) - (v2f)cameraPosition
			);

			m_tileSprite.setScale(TEXTURE_SCALE * currentCrop->growth, TEXTURE_SCALE * currentCrop->growth);
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
