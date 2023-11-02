#include "AtlasManager.h"

sf::Texture AtlasManager::Atlases[];

std::string AtlasManager::atlasFilenames[] = {
	"Assets/atlas.png",
	"Assets/entities.png",
	"Assets/ui.png",
	"Assets/items.png"
};

bool AtlasManager::LoadAtlases()
{
	printf("Loading Atlases...\n");

	int counter = 0;

	for (int atlasId = 0; atlasId < MAX_ATLASES; atlasId++)
	{
		counter += AtlasManager::Atlases[atlasId].loadFromFile(atlasFilenames[atlasId]);
	}

	return counter == MAX_ATLASES;
}

sf::Texture* AtlasManager::GetAtlas(AtlasTextureID atid)
{
	return &AtlasManager::Atlases[atid];
}
