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

	static time_t getTimestamp();
	static void Log(const std::string& text);

	static v2f vector2fSqrt(const v2f& value);
};