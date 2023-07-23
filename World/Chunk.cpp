#include "Chunk.h"
#include "Tile.h"
#include "TileRegistry.h"

sf::Sprite Chunk::m_tileSprite = Sprite(*AtlasManager::GetAtlas(AtlasTextureID::Tiles), IntRect(0, 0, 16, 16));

Chunk::Chunk(const v2f& position)
{
	m_position = position;


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
		m_position.x < (cameraPosition.x + WINDOW_WIDTH) / (TILE_SIZE * TEXTURE_SCALE) 
		&& m_position.y < (cameraPosition.y + WINDOW_HEIGHT) / (TILE_SIZE * TEXTURE_SCALE)
		&& m_position.x > ((cameraPosition.x - WINDOW_WIDTH) / (TILE_SIZE * TEXTURE_SCALE))
		&& m_position.y > ((cameraPosition.y - WINDOW_HEIGHT) / (TILE_SIZE * TEXTURE_SCALE));
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
				Tile currentTile = TileRegistry::Tiles[tileID];
				int scaledTileSize = TILE_SIZE * TEXTURE_SCALE;

				AtlasID tileAtlasId = (z == 0) ? currentTile.groundId : currentTile.textureId;

				m_tileSprite.setTextureRect(IntRect(tileAtlasId.x * TILE_SIZE, tileAtlasId.y * TILE_SIZE, TILE_SIZE, TILE_SIZE));

				m_tileSprite.setPosition(
					v2f(
						(m_position.x + x) * scaledTileSize,
						(m_position.y + y) * scaledTileSize
					) - (v2f) cameraPosition
				);

				m_tileSprite.setScale(TEXTURE_SCALE, TEXTURE_SCALE);

				window->draw(m_tileSprite);
			}
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

	return m_tiles[layer][(int) position.y][(int) position.x];
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
