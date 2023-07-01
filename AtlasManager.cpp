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

	for (int atID = 0; atID < MAX_ATLASES; atID++)
	{
		counter += AtlasManager::Atlases[atID].loadFromFile(atlasFilenames[atID]);
	}

	return counter == MAX_ATLASES;
}

sf::Texture* AtlasManager::GetAtlas(AtlasTextureID atid)
{
	return &AtlasManager::Atlases[atid];
}
