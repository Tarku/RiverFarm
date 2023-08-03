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
public:

	static int RandInt(int min, int max);

	static time_t GetTimestamp();
	static void Log(const std::string& text);

	static v2f vector2fSqrt(const v2f& value);

	static float Length(const v2f& value);
	static float LengthSquared(const v2f& value);
	static float Distance(const v2f& first, const v2f& second);

	static float DistanceSquared(const v2f& first, const v2f& second);

	static v2f Normalize(const v2f& value);
};