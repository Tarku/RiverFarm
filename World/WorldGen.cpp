#include "WorldGen.h"
#include "../Game.h"
#include "../Utils.h"

siv::PerlinNoise WorldGen::perlin = siv::PerlinNoise();

void WorldGen::Initialize()
{
	perlin.reseed(Game::Seed);
}

double WorldGen::GetHumidityValue(int x, int y)
{

	double biomeValue = perlin.octave2D_01(x * (1.f / 128), y * (1.f / 187), 2);
	double biomeValue2 = perlin.octave2D_01(x * (1.f / 28), y * (1.f / 69), 1);

	double finalBiomeValue = (biomeValue + biomeValue2) * 0.5f;

	return finalBiomeValue;
}
double WorldGen::GetHeatValue(int x, int y)
{

	double biomeValue = perlin.octave2D_01(x * (1.f / 289), y * (1.f / 320), 2);
	double biomeValue2 = perlin.octave2D_01(x * (1.f / 28), y * (1.f / 68), 3);
	double finalBiomeValue = (biomeValue + biomeValue2) * 0.5f;

	return finalBiomeValue;
}

WorldGen::BiomeType WorldGen::GetBiome(int x, int y)
{

	int humidityValue = static_cast<int>( (1 - GetHumidityValue(x, y) * lookupResolution));
	int heatValue = static_cast<int>(GetHeatValue(x, y) * lookupResolution);

	humidityValue = Utils::Clamp<int>(humidityValue, 0, lookupResolution);
	heatValue = Utils::Clamp<int>(heatValue, 0, lookupResolution);
		
	return biomeLookup[humidityValue][heatValue];
	

}

double WorldGen::GetHeightmapValue(int x, int y)
{

	double heightmapValue1 = perlin.octave2D_01(x * (1.f / 16), y * (1.f / 16), 2);
	double heightmapValue2 = perlin.octave2D_01(x * (1.f / 32), y * (1.f / 32), 1);
	double heightmapValue3 = perlin.octave2D_01(x * (1.f / 64), y * (1.f / 64), 2);
	double heightmapValue4 = perlin.octave2D_01(x * (1.f / 128), y * (1.f / 128), 4);

	double finalHeightmapValue =
		(
			heightmapValue1
			+ heightmapValue2
			+ heightmapValue3
			) / 3.0
		-
		heightmapValue4 / 8;
		

	
	return finalHeightmapValue;
}

double WorldGen::GetErosionValue(int x, int y)
{
	double erosionValue = perlin.octave2D_01(x * (1.f / 8) + x * (1.f / 16), y * (1.f / 16) - y * (1.f / 8), 5);

	return erosionValue;
}