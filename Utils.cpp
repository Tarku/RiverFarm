#include "Utils.h"

std::mt19937 Utils::m_rng;

time_t Utils::getTimestamp()
{
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void Utils::Log(const std::string& text)
{
	printf("%s\n", text.c_str());
}

v2f Utils::vector2fSqrt(const v2f& value)
{
	return v2f(sqrt(value.x), sqrt(value.y));
}


int Utils::RandInt(int min, int max)
{
	auto distrib = std::uniform_int_distribution<int>(min, max);
	return distrib(m_rng);
}
