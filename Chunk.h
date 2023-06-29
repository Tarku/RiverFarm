#pragma once

#include "Shared.h"

class Chunk
{
private:
	uchar m_tiles[MAP_LEVELS][CHUNK_WIDTH][CHUNK_HEIGHT];
	v2f m_position;

public:
	Chunk(const v2f& position);

	bool CanBeRendered(const v2f& cameraPosition);

	void Draw(sf::RenderWindow* window, const v2f& cameraPosition);

	void SetTile(const v2f& position, int layer, uchar tileID);
	void SetTile(const v3f& position, uchar tileID);

	uchar TileAt(const v2f& position, int layer);
	uchar TileAt(const v3f& position);

	bool IsEmptyAt(const v2f& position, int layer);
	bool IsEmptyAt(const v3f& position);
};

