#include "Chunk.h"
#include "Tiles/Tile.h"
#include "Tiles/TileRegistry.h"
#include "Crop.h"
#include "World.h"
#include "WorldGen.h"
#include "../Interface.h"
#include "BiomeRegistry.h"

sf::Sprite Chunk::m_tileSprite = Sprite(*AtlasManager::GetAtlas(AtlasTextureID::Tiles));

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
		position.x * CHUNK_WIDTH < (cameraPosition.x + WINDOW_WIDTH) / SCALED_TILE_SIZE
		&& position.y * CHUNK_HEIGHT < (cameraPosition.y + WINDOW_HEIGHT) / SCALED_TILE_SIZE
		&& position.x * CHUNK_WIDTH >(cameraPosition.x - WINDOW_WIDTH) / SCALED_TILE_SIZE
		&& position.y * CHUNK_HEIGHT >(cameraPosition.y - WINDOW_HEIGHT) / SCALED_TILE_SIZE;
}
void Chunk::DoWorldGen()
{
	for (int y = 0; y < CHUNK_WIDTH; y++)
	{
		for (int x = 0; x < CHUNK_HEIGHT; x++)
		{
			BiomeID biomeID = WorldGen::GetBiome(position.x * CHUNK_WIDTH + x, position.y * CHUNK_HEIGHT + y);
			Biome* biome = BiomeRegistry::Biomes.at(biomeID);

			double height = WorldGen::GetHeightmapValue(position.x * CHUNK_WIDTH + x, position.y * CHUNK_HEIGHT + y);

			// Generate ground tile

			if (height < 0.4)
			{
				SetTile(
					v3f(x, y, 0),
					TileID::Water
				);
				continue;
			}
			if (height >= 0.4 && height < 0.45)
			{
				SetTile(
					v3f(x, y, 0),
					TileID::Sand
				);
				continue;
			}

			TileID groundTile = biome->groundTile;

			// Generate water holes

			if (biomeID == BiomeID::Rainforest)
			{
				if (Utils::RandInt(0, 8) == 0)
				{
					groundTile = TileID::Water;
				}
			}
			if (biomeID == BiomeID::Steppe || biomeID == BiomeID::Savanna)
			{

				if (Utils::RandInt(0, 10) == 0)
				{
					groundTile = TileID::Dirt;
				}
			}

			SetTile(
				v3f(x, y, 0),
				groundTile
			);

			if (biome->treeScarcity == -1)
				continue;
				
			// Generate trees
			TileID treeTile = biome->treeTile;

			if (Utils::RandInt(0, biome->treeScarcity) == 0 && groundTile != TileID::Water)
			{
				SetTile(
					v3f(x, y, 1),
					treeTile
				);
			}
		}
	}

	state = ChunkState::Unloaded;

	AddDecorations();
	
	CowEntity* cow = new CowEntity(
		v2f(
			CHUNK_WIDTH * position.x + Utils::RandInt(0, CHUNK_WIDTH),
			CHUNK_HEIGHT * position.y + Utils::RandInt(0, CHUNK_HEIGHT)
		)
	);

		m_world->AddEntity(cow);
}

void Chunk::AddDecorations()
{
	if (Utils::RandInt(0, 100) == 0)
	{

		int roomWidth = Utils::RandInt(5, 14);
		int roomHeight = Utils::RandInt(5, 14);

		int roomX = Utils::RandInt(0, CHUNK_WIDTH - roomWidth);
		int roomY = Utils::RandInt(0, CHUNK_HEIGHT - roomHeight);

		double topLeftHeight = WorldGen::GetHeightmapValue(position.x * CHUNK_WIDTH + roomX, position.y * CHUNK_HEIGHT + roomY);
		double topRightHeight = WorldGen::GetHeightmapValue(position.x * CHUNK_WIDTH + roomX + roomWidth, position.y * CHUNK_HEIGHT + roomY);
		double bottomRightHeight = WorldGen::GetHeightmapValue(position.x * CHUNK_WIDTH + roomX + roomWidth, position.y * CHUNK_HEIGHT + roomY + roomHeight);
		double bottomLeftHeight = WorldGen::GetHeightmapValue(position.x * CHUNK_WIDTH + roomX, position.y * CHUNK_HEIGHT + roomY + roomHeight);

		double avgHeight = (topLeftHeight + topRightHeight + bottomLeftHeight + bottomRightHeight) / 4.0;

		// If the height of the decoration is below water level, then don't do it
		if (avgHeight < 0.4)
		{
			return;
		}

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

		int doorAmount = Utils::RandInt(1, 3);
		int lastRandomSide = -1;

		for (int doorId = 0; doorId < doorAmount; doorId++)
		{
			int doorX = Utils::RandInt(1, roomWidth - 2);
			int doorY = Utils::RandInt(1, roomHeight - 2);

			int randomSide = Utils::RandInt(0, 3);

			while (randomSide == lastRandomSide)
			{
				randomSide = Utils::RandInt(0, 3);
			}

			switch (randomSide)
			{
			case 0:
				SetTile(v2f(roomX + doorX, roomY + roomHeight - 1), 1, TileID::Door);
				break;
			case 1:
				SetTile(v2f(roomX + roomWidth - 1, roomY + doorY), 1, TileID::Door);
				break;
			case 2:
				SetTile(v2f(roomX, roomY + doorY), 1, TileID::Door);
				break;
			case 3:
				SetTile(v2f(roomX + doorX, roomY), 1, TileID::Door);
				break;
			}

			lastRandomSide = randomSide;
		}
	}
}

void Chunk::Update(float dt)
{

	// Random tick update

	const int blocksToUpdate = 
		(CHUNK_WIDTH * CHUNK_HEIGHT * MAP_LEVELS) / 70;

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
				//Utils::Log(std::format("Drawing tile at {} {} {}", x, y, z));

				Tile* currentTile = TileRegistry::Tiles.at(tileID);

				if (currentTile == nullptr)
				{
					continue;
				}

				AtlasID tileAtlasId = currentTile->GetContextTextureID(
					v2f(
						position.x * CHUNK_WIDTH + x,
						position.y * CHUNK_HEIGHT + y
					),
					m_world,
					z
				);

				m_tileSprite.setTextureRect(
					
					IntRect(
						v2i(
							floor(tileAtlasId.x * TILE_SIZE),
							floor(tileAtlasId.y * TILE_SIZE)
						),
						v2i(
							TILE_SIZE,
							TILE_SIZE
						)
					));

				auto drawPos =
					v2f(
						(position.x * CHUNK_WIDTH + x) * SCALED_TILE_SIZE,
						(position.y * CHUNK_HEIGHT + y) * SCALED_TILE_SIZE
					) - (v2f)cameraPosition;

				//Utils::Log(std::format("Drawing tile at screen pos X: {}, Y: {}", drawPos.x, drawPos.y));

				m_tileSprite.setPosition(
					drawPos
				);

				m_tileSprite.setScale(v2f(TEXTURE_SCALE, TEXTURE_SCALE));

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

			// draw the first two pickets behind the crop

			AtlasID cropDelimiter = { 4, 3 };
			m_tileSprite.setTextureRect(
				IntRect(
					v2i(cropDelimiter.x * TILE_SIZE, cropDelimiter.y * TILE_SIZE),
					v2i(TILE_SIZE, TILE_SIZE / 2)
				)
			);

			m_tileSprite.setPosition(
				v2f(
					(position.x + x) * SCALED_TILE_SIZE,
					(position.y + y) * SCALED_TILE_SIZE
				) - (v2f)cameraPosition
			);

			m_tileSprite.setScale(v2f(TEXTURE_SCALE, TEXTURE_SCALE));
			window->draw(m_tileSprite);

			// draw the actual crop

			m_tileSprite.setTextureRect(IntRect(v2i(tileAtlasId.x * TILE_SIZE, tileAtlasId.y * TILE_SIZE), v2i(TILE_SIZE, TILE_SIZE)));

			float g = currentCrop->growth;

			m_tileSprite.setPosition(
				v2f(
					(position.x + x + (0.5f * (1 - g))) * SCALED_TILE_SIZE,
					(position.y + y + (0.5f * (1 - g))) * SCALED_TILE_SIZE
				) - (v2f)cameraPosition
			);

			m_tileSprite.setScale(v2f(TEXTURE_SCALE * g, TEXTURE_SCALE * g));
			window->draw(m_tileSprite);


			// draw the last two pickets in front of the crop
			m_tileSprite.setTextureRect(
				IntRect(
					v2i(cropDelimiter.x * TILE_SIZE, cropDelimiter.y * TILE_SIZE + TILE_SIZE / 2),
					v2i(TILE_SIZE, TILE_SIZE / 2)
				)
			);

			m_tileSprite.setPosition(
				v2f(
					(position.x + x) * SCALED_TILE_SIZE,
					(position.y + y) * SCALED_TILE_SIZE + SCALED_TILE_SIZE / 2
				) - (v2f)cameraPosition
			);

			m_tileSprite.setScale(v2f(TEXTURE_SCALE, TEXTURE_SCALE));
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
		return 0;

	return m_tiles[layer][(int)position.y][(int)position.x];
}

uchar Chunk::TileAt(const v3f& position)
{
	if (position.x < 0 || position.y < 0 || position.z < 0 || position.x >= CHUNK_WIDTH || position.y >= CHUNK_HEIGHT || position.z > 1)
		return 0;

	return m_tiles[(int) position.z][(int)position.y][(int)position.x];
}

bool Chunk::IsEmptyAt(const v2f& position, int layer)
{
	return false;
}

bool Chunk::IsEmptyAt(const v3f& position)
{
	return false;
}
