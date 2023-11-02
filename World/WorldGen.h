#include "../Includes/PerlinNoise.hpp"
#include "Tiles/TileRegistry.h"


class WorldGen
{
public:
	enum class BiomeType
	{
		Plains,
		ColdPlains,
		Savanna,
		Forest,
		Rainforest,
		Taiga,
		Desert
	};

	inline static std::map<BiomeType, int> BiomeTreeScarcity =
	{
		{ BiomeType::Plains, 28 },
		{ BiomeType::ColdPlains, 32 },
		{ BiomeType::Savanna, 32 },
		{ BiomeType::Forest, 4 },
		{ BiomeType::Rainforest, 3 },
		{ BiomeType::Taiga, 5 },
		{ BiomeType::Desert, 32 }
	};
	inline static std::map<BiomeType, TileID> BiomeTreeType =
	{
		{ BiomeType::Plains, TileID::Tree },
		{ BiomeType::ColdPlains, TileID::FirTree },
		{ BiomeType::Savanna, TileID::Cactus },
		{ BiomeType::Forest, TileID::Tree },
		{ BiomeType::Rainforest, TileID::MangroveTree },
		{ BiomeType::Taiga, TileID::FirTree },
		{ BiomeType::Desert, TileID::Cactus }
	};

private:
	inline static const int lookupResolution = 5;

	static siv::PerlinNoise perlin;

	inline static BiomeType biomeLookup[lookupResolution][lookupResolution] = {
		// heat is horizontal axis ----> 
		// humidity is vertical axis
		
		// Very cold			 Cold			  Temperate			 Warm					Hot
		{ BiomeType::Taiga,		 BiomeType::Taiga, BiomeType::Forest, BiomeType::Rainforest, BiomeType::Rainforest }, // Very humid

		{ BiomeType::Taiga, BiomeType::Forest, BiomeType::Forest, BiomeType::Forest, BiomeType::Rainforest }, // Humid

		{ BiomeType::ColdPlains, BiomeType::Plains, BiomeType::Plains, BiomeType::Savanna, BiomeType::Savanna }, // Normal

		{ BiomeType::ColdPlains, BiomeType::Plains, BiomeType::Plains, BiomeType::Savanna, BiomeType::Savanna }, // Arid

		{ BiomeType::ColdPlains, BiomeType::ColdPlains, BiomeType::Savanna, BiomeType::Desert, BiomeType::Desert }, // Very arid
	};

public:
	static void Initialize();

	static double GetHumidityValue(int x, int y);
	static double GetHeatValue(int x, int y);

	static BiomeType GetBiome(int x, int y);
	static double GetHeightmapValue(int x, int y);

	static double GetErosionValue(int x, int y);

};