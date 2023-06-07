#pragma once

#include "Shared.h"

enum AtlasTextureID
{
	Tiles,
	Entities,
	UI,
	Items
};

class AtlasManager
{
public:
	static sf::Texture Atlases[4];

	static std::string atlasFilenames[4];

	static bool LoadAtlases();
	static sf::Texture* GetAtlas(AtlasTextureID atid);
};