#include <string>
#include <format>

#include "../Shared.h"

struct WorldTime
{
	long long ticks;
	long long minutes;
	long long hours;
	long long days;
	long long weeks;
	long long months;
	long long years;

	inline WorldTime()
	{
		ticks = 0;
		minutes = 0;
		hours = 6;
		days = 0;
		weeks = 0;
		months = 0;
		years = 1000;
	}

	inline void Reset()
	{
		ticks = 0;
		minutes = 0;
		hours = 6;
		days = 0;
		weeks = 0;
		months = 0;
		years = 1000;
	}

	inline void Update()
	{
		ticks++;

		if (ticks == TimeConstants::TICKS_PER_MINUTE)
		{
			ticks = 0;
			minutes++;
		}

		if (minutes == TimeConstants::MINUTES_PER_HOUR)
		{
			minutes = 0;
			hours++;
		}

		if (hours == TimeConstants::HOURS_PER_DAY)
		{
			hours = 0;
			days++;
		}

		if (days == TimeConstants::DAYS_PER_WEEK)
		{
			days = 0;
			weeks++;
		}

		if (weeks == TimeConstants::WEEKS_PER_MONTH)
		{
			weeks = 0;
			months++;
		}

		if (months == TimeConstants::MONTHS_PER_YEAR)
		{
			months = 0;
			years++;
		}
	}

	inline std::string GetHourString()
	{
		std::string hourString = std::format("{}", hours + 1);
		std::string minutesString = std::format("{}", minutes + 1);

		if (hourString.size() == 1)
		{
			hourString.insert(0, std::string("0"));
		}
		if (minutesString.size() == 1)
		{
			minutesString.insert(0, std::string("0"));
		}

		return std::format("{}:{}", hourString, minutesString);
	}

	inline std::string GetDateString()
	{
		std::string yearString = std::format("{}", years);
		std::string monthString = std::format("{}", months + 1);
		std::string dayString = std::format("{}", days + 1);

		if (monthString.size() == 1)
		{
			monthString.insert(0, std::string("0"));
		}
		if (dayString.size() == 1)
		{
			dayString.insert(0, std::string("0"));
		}

		std::string hourString = std::format("{}", hours);
		std::string minutesString = std::format("{}", minutes);

		if (hourString.size() == 1)
		{
			hourString.insert(0, std::string("0"));
		}
		if (minutesString.size() == 1)
		{
			minutesString.insert(0, std::string("0"));
		}

		return std::format("{}/{}/{} {}:{}", yearString, monthString, dayString, hourString, minutesString);
	}
};