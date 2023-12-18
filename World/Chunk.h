#pragma once

#include "../Shared.h"

#include <vector>

class World;
class Crop;

struct Metadata
{
	unsigned int misc : 3;
	unsigned int binaryState : 1;
	unsigned int damage : 4;

	inline Metadata()
	{
		misc = 0b000;
		binaryState = 0b0;
		damage = 0b0000;
	}

	inline Metadata(unsigned int misc, unsigned int binaryState, unsigned int damage) : misc(misc), binaryState(binaryState), damage(damage)
	{

	}

	inline static unsigned int InvalidTileMisc = 0b1111;
};

struct TileStorage
{
	Metadata meta;

	uchar id;

	inline TileStorage() : meta(Metadata()), id(0)
	{

	}

	inline TileStorage(Metadata meta, uchar id) : meta(meta), id(id)
	{
		
	}
};

class Chunk : public std::enable_shared_from_this<Chunk>
{
private:
	static sf::Sprite m_tileSprite;

	TileStorage m_tiles[MAP_LEVELS][CHUNK_HEIGHT][CHUNK_WIDTH];
	Crop* m_crops[CHUNK_HEIGHT][CHUNK_WIDTH];

	World* m_world;


public:
	enum class ChunkState { Null, Loaded, Unloaded, Ungenerated };
	ChunkState state = ChunkState::Ungenerated;

	Chunk() = delete;
	Chunk(const v2f& position, World* world);

	inline ~Chunk()
	{
		std::cout << "Chunk got destroyed" << std::endl;
	}

	v2f position;

	bool CanBeRendered(const v2f& cameraPosition);
	void DoWorldGen();
	void AddDecorations();

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

	Metadata MetaAt(const v2f& position, int layer);
	void SetMeta(const v2f& position, int layer, Metadata metadata);

	bool IsEmptyAt(const v2f& position, int layer);
	bool IsEmptyAt(const v3f& position);
};

