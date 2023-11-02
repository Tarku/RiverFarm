#pragma once

#include "../Shared.h"

#include <vector>

class World;
class Crop;

class Chunk
{
private:
	static sf::Sprite m_tileSprite;

	uchar m_tiles[MAP_LEVELS][CHUNK_HEIGHT][CHUNK_WIDTH];
	Crop* m_crops[CHUNK_HEIGHT][CHUNK_WIDTH];

	World* m_world;


public:
	enum class ChunkState { Null, Loaded, Unloaded, Ungenerated };
	ChunkState state = ChunkState::Ungenerated;

	Chunk() = delete;
	Chunk(const v2f& position, World* world);

	v2f position;

	bool CanBeRendered(const v2f& cameraPosition);
	void DoWorldGen();

	void Update(float dt);

	void AddCrop(const v2f& inChunkPosition, Crop* crop);
	void RemoveCrop(const v2f& position);

	bool IsCrop(const v2f& position);
	Crop* CropAt(const v2f& position);

	void Draw(sf::RenderWindow* window, const v2f& cameraPosition);

	void SetTile(const v2f& position, int layer, uchar tileID);
	void SetTile(const v3f& position, uchar tileID);

	uchar TileAt(const v2f& position, int layer);
	uchar TileAt(const v3f& position);

	bool IsEmptyAt(const v2f& position, int layer);
	bool IsEmptyAt(const v3f& position);
};

