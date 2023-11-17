#include "../Includes/PerlinNoise.hpp"
#include "Tiles/TileRegistry.h"
#include "Biome.h"
#include "BiomeID.h"


class WorldGen
{
public:
private:
	inline static const int lookupResolution = 5;

	static siv::PerlinNoise perlin;

	inline static BiomeID biomeLookup[lookupResolution][lookupResolution] = {
		// heat is horizontal axis ----> 
		// humidity is vertical axis
		
		// Very cold			 Cold			  Temperate			 Warm					Hot
		{ BiomeID::Taiga,		 BiomeID::Taiga, BiomeID::Forest, BiomeID::Rainforest, BiomeID::Rainforest }, // Very humid

		{ BiomeID::Taiga, BiomeID::Forest, BiomeID::Forest, BiomeID::Forest, BiomeID::Rainforest }, // Humid

		{ BiomeID::Steppe, BiomeID::Plains, BiomeID::Plains, BiomeID::Savanna, BiomeID::Forest }, // Normal

		{ BiomeID::Steppe, BiomeID::Plains, BiomeID::Plains, BiomeID::Savanna, BiomeID::Savanna }, // Arid

		{ BiomeID::Steppe, BiomeID::Steppe, BiomeID::Savanna, BiomeID::Desert, BiomeID::Desert }, // Very arid
	};

public:
	static void Initialize();

	static double GetHumidityValue(int x, int y);
	static double GetHeatValue(int x, int y);

	static BiomeID GetBiome(int x, int y);
	static double GetHeightmapValue(int x, int y);

	static double GetErosionValue(int x, int y);

};