#include "WorldGen.h"
#include "../Game.h"
#include "../Utils.h"

siv::PerlinNoise WorldGen::perlin = siv::PerlinNoise();

double WorldGen::BeachLevel = 0.45;
double WorldGen::WaterLevel = 0.4;

void WorldGen::Initialize()
{
	perlin.reseed(Game::Seed);
}

double WorldGen::GetHumidityValue(int x, int y)
{

	double biomeValue = perlin.octave2D_01(x * (1.f / 128), y * (1.f / 128), 2);
	double biomeValue2 = perlin.octave2D_01(x * (1.f / 64), y * (1.f / 64), 1);

	double finalBiomeValue = (biomeValue + biomeValue2) * 0.35;

	return finalBiomeValue;
}
double WorldGen::GetHeatValue(int x, int y)
{

	double biomeValue = perlin.octave2D_01(x * (1.f / 256), y * (1.f / 256), 2);
	double biomeValue2 = perlin.octave2D_01(x * (1.f / 78), y * (1.f / 78), 3);
	double biomeValue3 = perlin.octave2D_01(x * (1.f / 128), y * (1.f / 128), 5);
	double finalBiomeValue = biomeValue3 * 0.7 + (biomeValue + biomeValue2) * 0.6;

	return finalBiomeValue;
}

BiomeID WorldGen::GetBiome(int x, int y)
{
	double height = GetHeightmapValue(x, y);



	int humidityValue = static_cast<int>( GetHumidityValue(x, y) * (lookupResolution));
	int heatValue = static_cast<int>(GetHeatValue(x, y) * (lookupResolution));

	int oceanLookupValue = static_cast<int>(GetHeatValue(x, y) * (oceanLookupResolution));
	int beachLookupValue = static_cast<int>(GetHeatValue(x, y) * (beachLookupResolution));

	humidityValue = Utils::Clamp<int>(humidityValue, 0, (lookupResolution + 1));
	heatValue = Utils::Clamp<int>(heatValue, 0, (lookupResolution + 1));

	if (height < WaterLevel)
		return oceanLookup[oceanLookupValue];
	else if (height < BeachLevel)
		return beachLookup[beachLookupValue];
		
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