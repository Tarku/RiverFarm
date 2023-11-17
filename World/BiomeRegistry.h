#include "BiomeID.h"
#include "Biome.h"

#include <vector>

class BiomeRegistry
{
public:
	inline static std::map<BiomeID, Biome*> Biomes = std::map<BiomeID, Biome*>
	{
		{ BiomeID::Plains, new Biome(std::string("Plains"), TileID::Grass, TileID::Tree, 28) },
		{ BiomeID::Steppe, new Biome(std::string("Steppe"), TileID::DryGrass, TileID::Air, -1)},
		{ BiomeID::Savanna, new Biome(std::string("Savanna"), TileID::DryGrass, TileID::AcaciaTree, 48)},
		{ BiomeID::Forest, new Biome(std::string("Forest"), TileID::Grass, TileID::Tree, 4)},
		{ BiomeID::Rainforest, new Biome(std::string("Rainforest"), TileID::SwampGrass, TileID::MangroveTree, 5)},
		{ BiomeID::Taiga, new Biome(std::string("Taiga"), TileID::Snow, TileID::FirTree, 5)},
		{ BiomeID::Desert, new Biome(std::string("Desert"), TileID::Sand, TileID::Cactus, 32)},
	};
};