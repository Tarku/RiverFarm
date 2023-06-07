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
