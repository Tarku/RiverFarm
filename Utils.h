#pragma once

#include <cstdint>
#include <chrono>
#include <string>
#include <random>
#include "Shared.h"

class Utils
{
private:
	static std::mt19937 m_rng;
	static sf::RenderWindow* m_window;

public:
	static void Initialize(sf::RenderWindow* window);

	inline static int RandInt(int min, int max)
	{
		auto distrib = std::uniform_int_distribution<int>(min, max);
		return distrib(m_rng);
	}
	
	inline static float RandFloat(float min, float max)
	{
		auto distrib = std::uniform_real_distribution<float>(min, max);
		return distrib(m_rng);
	}

	static time_t GetTimestamp();
	static void Log(const std::string& text);

	static v2f vector2fSqrt(const v2f& value);

	static v2f GetMousePosition();

	static float Length(const v2f& value);
	static float LengthSquared(const v2f& value);
	static float Distance(const v2f& first, const v2f& second);

	static float DistanceSquared(const v2f& first, const v2f& second);

	static v2f Normalize(const v2f& value);
};