#include "AtlasManager.h"

sf::Texture AtlasManager::Atlases[];

std::string AtlasManager::atlasFilenames[] = {
	"atlas.png",
	"entities.png",
	"ui.png",
	"items.png"
};

bool AtlasManager::LoadAtlases()
{
	printf("Loading Atlases...\n");

	int counter = 0;

	for (int atlasId = 0; atlasId < MAX_ATLASES; atlasId++)
	{
		counter += AtlasManager::Atlases[atlasId].loadFromFile(TEXTURES_PATH + atlasFilenames[atlasId]);
	}

	return counter == MAX_ATLASES;
}

sf::Texture* AtlasManager::GetAtlas(AtlasTextureID atid)
{
	return &AtlasManager::Atlases[atid];
}
