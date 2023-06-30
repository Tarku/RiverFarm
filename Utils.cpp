#include "Utils.h"

time_t Utils::getTimestamp()
{
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void Utils::Log(const std::string& text)
{
	printf("%s\n", text.c_str());
}