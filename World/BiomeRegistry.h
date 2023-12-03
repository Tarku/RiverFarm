#include "BiomeID.h"
#include "Biome.h"

#include <vector>

class BiomeRegistry
{
public:
	inline static std::map<BiomeID, Biome*> Biomes = std::map<BiomeID, Biome*>
	{
		{
			BiomeID::Plains,
			new Biome(
				string("Plains"),
				TileID::Grass,
				std::vector({
					TreeGenerationProperty(TileID::Tree, 6),
					TreeGenerationProperty(TileID::Flowers, 16)
				}),
				18
			)
		},
		{
			BiomeID::Steppe,
			new Biome(
				string("Steppe"),
				TileID::DryGrass,
				std::vector<TreeGenerationProperty>(),
				-1
			)
		},
		{
			BiomeID::Savanna,
			new Biome(
				string("Savanna"),
				TileID::DryGrass,
				std::vector({
					TreeGenerationProperty(TileID::Flowers, 2),
					TreeGenerationProperty(TileID::Tree, 8),
				}),
				48
			)
		},
		{
			BiomeID::Forest,
			new Biome(
				string("Forest"),
				TileID::Grass,
				std::vector({
					TreeGenerationProperty(TileID::Flowers, 2),
					TreeGenerationProperty(TileID::Tree, 6),
					TreeGenerationProperty(TileID::BirchTree, 1)
				}),
				3
			)
		},
		{
			BiomeID::ColdForest,
			new Biome(
				string("Cold forest"),
				TileID::Grass,
				std::vector({
					TreeGenerationProperty(TileID::Flowers, 2),
					TreeGenerationProperty(TileID::FirTree, 2),
					TreeGenerationProperty(TileID::BirchTree, 2)
				}),
				3
			)
		},
		{
			BiomeID::Rainforest,
			new Biome(
				string("Rainforest"),
				TileID::SwampGrass,
				std::vector({
					TreeGenerationProperty(TileID::Flowers, 1),
					TreeGenerationProperty(TileID::MangroveTree, 3)
				}),
				5
			)
		},
		{
			BiomeID::Taiga,
			new Biome(
				string("Taiga"),
				TileID::Snow,
				std::vector({
					TreeGenerationProperty(TileID::FirTree, 4),
					TreeGenerationProperty(TileID::BirchTree, 1),
				}),
				5
			)
		},
		{
			BiomeID::Desert,
			new Biome(
				string("Desert"),
				TileID::Sand,
				std::vector({
					TreeGenerationProperty(TileID::Cactus, 1),
				}),
				32
			)
		},
		{
			BiomeID::Beach,
			new Biome(
				string("Beach"),
				TileID::Sand,
				std::vector<TreeGenerationProperty>(),
				-1
			)
		},
		{ 
			BiomeID::ColdBeach,
			new Biome(
				std::string("Cold beach"),
				TileID::Gravel,
				std::vector<TreeGenerationProperty>(),
				-1
			)
		},
		{ 
			BiomeID::Ocean,
			new Biome(
				std::string("Ocean"),
				TileID::Water,
				std::vector<TreeGenerationProperty>(),
				-1
			)
		},
		{
			BiomeID::IceSheet,
			new Biome(
				std::string("Ice sheet"),
				TileID::Ice,
				std::vector<TreeGenerationProperty>(),
				-1
			)
		}
	};
};