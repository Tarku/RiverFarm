#include "Utils.h"

std::mt19937 Utils::m_rng;

time_t Utils::GetTimestamp()
{
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void Utils::Log(const std::string& text)
{
	printf("%s\n", text.c_str());
}

v2f Utils::vector2fSqrt(const v2f& value)
{
	return v2f(sqrtf(value.x), sqrtf(value.y));
}


int Utils::RandInt(int min, int max)
{
	auto distrib = std::uniform_int_distribution<int>(min, max);
	return distrib(m_rng);
}

float Utils::Distance(const v2f& first, const v2f& second)
{
	float dx = second.x - first.x;
	float dy = second.y - first.y;

	return sqrtf(dx * dx + dy * dy);
}

float Utils::Length(const v2f& value)
{
	return sqrtf(value.x * value.x + value.y * value.y);
}
float Utils::LengthSquared(const v2f& value)
{
	return value.x * value.x + value.y * value.y;
}

float Utils::DistanceSquared(const v2f& first, const v2f& second)
{
	float dx = second.x - first.x;
	float dy = second.y - first.y;

	return dx * dx + dy * dy;
}

v2f Utils::Normalize(const v2f& value)
{
	return value / Length(value);
}