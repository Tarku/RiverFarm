#include "Chunk.h"
#include "Tile.h"
#include "TileRegistry.h"

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
		m_position.x < (cameraPosition.x + WINDOW_WIDTH / 2) / (TILE_SIZE * TEXTURE_SCALE) 
		&& m_position.y < (cameraPosition.y + WINDOW_HEIGHT / 2) / (TILE_SIZE * TEXTURE_SCALE)
		&& m_position.x > ((cameraPosition.x - WINDOW_WIDTH * 2) / (TILE_SIZE * TEXTURE_SCALE))
		&& m_position.y > ((cameraPosition.y - WINDOW_HEIGHT * 2) / (TILE_SIZE * TEXTURE_SCALE));
}

void Chunk::Draw(sf::RenderWindow* window, const v2f& cameraPosition)
{
	Sprite* s = new Sprite(*AtlasManager::GetAtlas(AtlasTextureID::Tiles), IntRect(0, 0, 16, 16));
	for (int z = 0; z < MAP_LEVELS; z++)
	{
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			for (int x = 0; x < CHUNK_WIDTH; x++)
			{

				Tile m_currentTile = TileRegistry::Tiles[TileAt(v2f(x, y), z)];
				int scaledTileSize = TILE_SIZE * TEXTURE_SCALE;

				s->setTextureRect(IntRect(m_currentTile.id.x * 16, m_currentTile.id.y * 16, 16, 16));

				s->setPosition(
					v2f(
						(m_position.x + x) * scaledTileSize,
						(m_position.y + y) * scaledTileSize
					) - cameraPosition
				);

				s->setScale(TEXTURE_SCALE, TEXTURE_SCALE);

				window->draw(*s);
			}
		}
	}
	delete s;

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
