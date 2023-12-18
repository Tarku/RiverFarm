#include "World.h"
#include "WorldGen.h"
#include "Tiles/Tile.h"
#include "Tiles/TileRegistry.h"
#include "../Utils.h"
#include "../Game.h"
#include "../Scenes/GameScene.h"

#include <algorithm>

std::list<Entity*> World::WorldEntities;
std::list<Entity*> World::EntitiesToDelete;

World::World()
{
}

void World::ResetWorld()
{
	DoWorldGen();

	worldTime.Reset();
	Inventory::Reset();

	WorldEntities.clear();

}

std::tuple<v2f, v2f> World::WorldToChunkPosition(const v2f& worldPosition)
{

	float chunkY = worldPosition.y / (float)CHUNK_HEIGHT;
	float chunkX = worldPosition.x / (float)CHUNK_WIDTH;

	int chunkPosX = abs (chunkX - (int)chunkX) * CHUNK_WIDTH;
	int chunkPosY = abs (chunkY - (int)chunkY) * CHUNK_HEIGHT;

	return std::make_tuple(
		v2f(
			(int) chunkX,
			(int) chunkY),
		v2f(
			(int) chunkPosX,
			(int) chunkPosY
		)
	);
}
v2f World::ChunkToWorldPosition(const v2f& chunkPosition, const v2f& inChunkPosition)
{

	int worldY = static_cast<int>(chunkPosition.y + inChunkPosition.y);
	int worldX = static_cast<int>(chunkPosition.x + inChunkPosition.x);

	return v2f(worldX, worldY);
}


void World::AddDecorations()
{
		
}


void World::SaveWorldToImage()
{
	int minX = 0xfffffff, maxX = -0xfffffff, minY = 0xfffffff, maxY = -0xfffffff;

	for (auto& chunk : Chunks)
	{
		if (chunk->position.x < minX)
			minX = chunk->position.x;
		if (chunk->position.x > maxX)
			maxX = chunk->position.x;

		if (chunk->position.y < minY)
			minY = chunk->position.y;
		if (chunk->position.y > maxY)
			maxY = chunk->position.y;
	}

	int imageWidth = (maxX - minX) + 1;
	int imageHeight = (maxY - minY) + 1;

	Utils::Log(std::format("Output image width: {}, height: {}", imageWidth * CHUNK_WIDTH, imageHeight * CHUNK_HEIGHT));

	Utils::Log("Saving world to image...");
	Image tileAtlasCopy = AtlasManager::GetAtlas(AtlasTextureID::Tiles)->copyToImage();

	Image outputImage;
	outputImage.create(v2u(imageWidth * CHUNK_WIDTH, imageHeight * CHUNK_HEIGHT));

	for (auto& chunk : Chunks)
	{
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			for (int x = 0; x < CHUNK_WIDTH; x++)
			{
				Tile* tile = TileRegistry::Tiles.at(chunk->TileAt(v3f(x, y, 0)));
				Tile* topTile = TileRegistry::Tiles.at(chunk->TileAt(v3f(x, y, 1)));

				Color tilePx = tileAtlasCopy.getPixel(
					v2u(
						tile->groundId.x * TILE_SIZE,
						tile->groundId.y * TILE_SIZE
					)
				);

				if (chunk->TileAt(v3f(x, y, 0)) == TileID::Air)
				{
					tilePx = Color(0, 0, 0, 255);
				}

				Color finalPx;

				if (chunk->TileAt(v3f(x, y, 1)) == TileID::Air)
				{
					finalPx = tilePx;
				}
				else
				{
					Color topTilePx = tileAtlasCopy.getPixel(
						v2u(
							topTile->textureId.x * TILE_SIZE,
							topTile->textureId.y * TILE_SIZE
						)
					);

					finalPx = Color(
						(uint8_t)((tilePx.r + topTilePx.r) * 0.5f),
						(uint8_t)((tilePx.g + topTilePx.g) * 0.5f),
						(uint8_t)((tilePx.b + topTilePx.b) * 0.5f)
					);
				}


				int pixelX = ((int) chunk->position.x - minX) * CHUNK_WIDTH + x;
				int pixelY = ((int) chunk->position.y - minY) * CHUNK_HEIGHT + y;

				// Utils::Log(std::format("Pixel X, Y: {}, {}", pixelX, pixelY));

				outputImage.setPixel(
					v2u(
						pixelX,
						pixelY
					),
					finalPx
				);
			}
		}
	}

	bool couldSave = outputImage.saveToFile("world.png");
	
	if (couldSave)
		Utils::Log("World saved.");
	else
		Utils::Log("World couldn't be saved.");
}

void World::DoWorldGen()
{

	Game::Seed = Utils::GetTimestamp();
	WorldGen::Initialize();

	Chunks.clear();

	v2f playerSpawnCoords = { MAP_WIDTH / 2.f + Utils::RandInt(-CHUNK_WIDTH / 2.f, CHUNK_WIDTH / 2.f), MAP_HEIGHT / 2.f + Utils::RandInt(-CHUNK_HEIGHT / 2.f, CHUNK_HEIGHT / 2.f) };

	auto spawnChunkCoordsTuple = WorldToChunkPosition(playerSpawnCoords);



	v2f spawnChunkCoords = std::get<0>(spawnChunkCoordsTuple);

	Utils::Log(std::format("Player spawn coordinates (in chunk space):\n X : {} | Y : {}", spawnChunkCoords.x, spawnChunkCoords.y));

	int radius = 2;

	for (int oy = -radius; oy <= radius; oy++)
	{
		for (int ox = -radius; ox <= radius; ox++)
		{

			Chunks.push_back(
				std::make_shared<Chunk>(spawnChunkCoords + v2f(ox, oy), this)
			);
		}
	}


	time_t worldgenStartTime = Utils::GetTimestamp();

	for (auto& chunk : Chunks)
	{
		chunk->DoWorldGen();
		
	}

	AddDecorations();



	// Add starting room at player spawn

	int spawnRoomHalfWidth = Utils::RandInt(3, 5);
	int spawnRoomHalfHeight = Utils::RandInt(3, 5);

	for (int yO = -spawnRoomHalfHeight; yO <= spawnRoomHalfHeight; yO++)
	{
		for (int xO = -spawnRoomHalfWidth; xO <= spawnRoomHalfWidth; xO++)
		{
			SetTile(v2f(playerSpawnCoords.x + xO, playerSpawnCoords.y + yO), 0, TileID::StonebrickWall, false);

			if (yO == -spawnRoomHalfHeight || xO == -spawnRoomHalfWidth || yO == spawnRoomHalfHeight || xO == spawnRoomHalfWidth)
			{
				SetTile(v2f(playerSpawnCoords.x + xO, playerSpawnCoords.y + yO), 1, TileID::StonebrickWall, false);
			}
			else {

				SetTile(v2f(playerSpawnCoords.x + xO, playerSpawnCoords.y + yO), 1, TileID::Air, false);
			}

			// Generate air for doorways
			SetTile(v2f(playerSpawnCoords.x + spawnRoomHalfWidth, playerSpawnCoords.y), 1, TileID::Door);
			SetTile(v2f(playerSpawnCoords.x - spawnRoomHalfWidth, playerSpawnCoords.y), 1, TileID::Door);
			SetTile(v2f(playerSpawnCoords.x, playerSpawnCoords.y + spawnRoomHalfHeight), 1, TileID::Door);
			SetTile(v2f(playerSpawnCoords.x, playerSpawnCoords.y - spawnRoomHalfHeight), 1, TileID::Door);
		}
	}

	time_t worldgenEndTime = Utils::GetTimestamp();
	time_t worldgenTotalTime = worldgenEndTime - worldgenStartTime;

	GameScene::Player->position = playerSpawnCoords;

	printf("World generation time: %d microseconds.\n", (int) worldgenTotalTime);
}

void World::UpdateChunkList()
{
	auto playerChunkPositionTuple = WorldToChunkPosition(GameScene::Player->position);

	v2f playerChunkPosition = std::get<0>(playerChunkPositionTuple);

	v2f minPosition = v2f(playerChunkPosition.x - 1, playerChunkPosition.y - 1);
	v2f maxPosition = v2f(playerChunkPosition.x + 1, playerChunkPosition.y + 1);

	for (int yo = -1; yo <= 1; yo++)
	{
		for (int xo = -1; xo <= 1; xo++)
		{
			v2f localPlayerChunkPosition = v2f(playerChunkPosition.x + xo, playerChunkPosition.y + yo);

			std::shared_ptr<Chunk> chunkAtPos = GetChunk(localPlayerChunkPosition);

			if (chunkAtPos == nullptr)
			{
				chunkAtPos = std::make_shared<Chunk>(v2f((int)localPlayerChunkPosition.x, (int)localPlayerChunkPosition.y), this);
				chunkAtPos->DoWorldGen();
				Chunks.push_back(chunkAtPos);
			}
		}
	}

	// A list containing the chunks that are going to be kept in the next iteration. Get swapped with the current list;
	std::vector<std::shared_ptr<Chunk>> keptChunks = std::vector<std::shared_ptr<Chunk>>();

	for (auto it = Chunks.begin(); it != Chunks.end(); it++)
	{
		std::shared_ptr<Chunk> chunkToCheck = *it;

		bool condition =
			chunkToCheck->position.x < minPosition.x
			|| chunkToCheck->position.y < minPosition.y
			|| chunkToCheck->position.x > maxPosition.x
			|| chunkToCheck->position.y > maxPosition.y;

		if (!condition)
		{
			keptChunks.push_back(chunkToCheck);
		}
	}

	Chunks.swap(keptChunks);
}

int World::DrawChunks(RenderWindow* window, const v2f& cameraPosition, bool drawChunkBorders)
{
	int chunksDrawn = 0;
	Vertex vertices[4] = {};

	v2f cameraChunkPosition = v2f((cameraPosition.x / SCALED_TILE_SIZE) / CHUNK_WIDTH, (cameraPosition.y / SCALED_TILE_SIZE) / CHUNK_HEIGHT);

	for (auto& chunk : Chunks)
	{
		int chunkX = chunk->position.x;
		int chunkY = chunk->position.y;


			if (chunk->CanBeRendered(cameraPosition))
			{
				chunk->Draw(window, cameraPosition);
				// Utils::Log("Chunk can be rendered.");
				chunksDrawn++;
			}

			if (drawChunkBorders)
			{

				vertices[0] = Vertex(v2f(chunkX * SCALED_TILE_SIZE * CHUNK_WIDTH, chunkY * SCALED_TILE_SIZE * CHUNK_WIDTH) - cameraPosition);
				vertices[1] = Vertex(v2f((chunkX + 1) * SCALED_TILE_SIZE * CHUNK_WIDTH, chunkY * SCALED_TILE_SIZE * CHUNK_WIDTH) - cameraPosition);

				window->draw(vertices, 2, sf::PrimitiveType::Lines);



				vertices[0] = Vertex(v2f(chunkX * SCALED_TILE_SIZE * CHUNK_WIDTH, chunkY * SCALED_TILE_SIZE * CHUNK_WIDTH) - cameraPosition);
				vertices[1] = Vertex(v2f(chunkX * SCALED_TILE_SIZE * CHUNK_WIDTH, (chunkY + 1) * SCALED_TILE_SIZE * CHUNK_WIDTH) - cameraPosition);

				window->draw(vertices, 2, sf::PrimitiveType::Lines);
			}
		
	}

	return chunksDrawn;
	// printf("%d chunks drawn.\n", chunksDrawn);
}

void World::Update(const v2f& cameraPosition, float dt)
{
	worldTime.Update();

	for (auto entity : WorldEntities)
	{
		entity->Update(this, dt);
	}

	for (auto entity : EntitiesToDelete)
	{
		RemoveEntity(entity);

	}
	UpdateChunkList();

	for (auto& chunk : Chunks)
	{
		if (chunk.get() != 0)
			chunk->Update(dt);
	}


}

std::shared_ptr<Chunk> World::GetChunk(const v2f& position)
{
	for (auto& chunk : Chunks)
	{
		if ((int)chunk->position.x == (int)position.x && (int)chunk->position.y == (int)position.y)
		{
			return chunk;
		}
	}
	return nullptr;
}

bool World::InBounds(const v2f& position, int layer)
{
	auto chunkPositionTuple = WorldToChunkPosition(position);

	return GetChunk(std::get<0>(chunkPositionTuple)) != nullptr;
}

void World::SetMeta(const v2f& position, int layer, Metadata metadata)
{

	if (!InBounds(position, layer))
		return;

	auto chunkPosTuple = WorldToChunkPosition(position);

	auto worldChunkPosition = std::get<0>(chunkPosTuple);
	auto inChunkOffset = std::get<1>(chunkPosTuple);

	std::shared_ptr<Chunk> chunk = GetChunk(worldChunkPosition);

	if (chunk == nullptr)
		return;

	chunk->SetMeta(inChunkOffset, layer, metadata);




}

Metadata World::MetaAt(const v2f& position, int layer)
{

	if (!InBounds(position, layer)) return Metadata(Metadata::InvalidTileMisc, 0, 0);

	auto chunkPosTuple = WorldToChunkPosition(position);

	auto worldChunkPosition = std::get<0>(chunkPosTuple);
	auto inChunkOffset = std::get<1>(chunkPosTuple);

	std::shared_ptr<Chunk> chunk = GetChunk(worldChunkPosition);

	if (chunk == nullptr) return Metadata(Metadata::InvalidTileMisc, 0, 0);

	return chunk->MetaAt(inChunkOffset, layer);

}

unsigned char World::TileAt(const v2f& position, int layer)
{
	if (!InBounds(position, layer)) return 0;

	return World::TileAt(position.x, position.y, layer);
}

unsigned char World::TileAt(int x, int y, int layer)
{
	if (!InBounds(Vector2f(x, y), layer)) return 0;

	auto chunkPosition = std::get<0>(WorldToChunkPosition(v2f(x, y)));
	auto chunkPositionOffset = std::get<1>(WorldToChunkPosition(v2f(x, y)));

	std::shared_ptr<Chunk> chunk = GetChunk(chunkPosition);

	if (chunk == nullptr) return 0;

	return chunk->TileAt(chunkPositionOffset, layer);
}

void World::SetTile(const v2f& position, int layer, unsigned char tileID, bool updateBlocks )
{
	if (!InBounds(position, layer)) return;

	auto chunkPosition = std::get<0>(WorldToChunkPosition(position));
	auto chunkPositionOffset = std::get<1>(WorldToChunkPosition(position));


	std::shared_ptr<Chunk> chunk = GetChunk(chunkPosition);

	if (chunk == nullptr) return;

	chunk->SetTile(chunkPositionOffset, layer, tileID);

	if (updateBlocks)
		TileRegistry::Tiles[tileID]->OnUpdate(position, this, layer);
}

void World::AddItemEntity(const v2f& position, ItemID itemID, int amount)
{
	for (int i = 0; i < amount; i++)
	{

		ItemEntity* newEntity = new ItemEntity(position, itemID);

		WorldEntities.push_back(newEntity);

	}
}
void World::AddEntity(Entity* entity)
{
	WorldEntities.push_back(entity);
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