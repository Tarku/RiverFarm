#pragma once

#include <cstdint>
#include <chrono>
#include <string>

class Utils
{
public:
	static time_t getTimestamp();
	static void Log(const std::string& text);
};