#include "Time.h"
#include <algorithm>

void maketime(Time & time)
{
	int totalSeconds;

	totalSeconds += time.year * 365 * 24 * 60 * 60
		+ ((int)(time.year / 4) + 1) * 24 * 60 * 60;
	
	for (int i = 0; i <= std::abs(time.mon); i++) totalSeconds += dayCount(i, time.year % 4 == 0) * 24 * 60 * 60 * (time.mon < 0 ? -1 : 1);

	totalSeconds += time.day * 24 * 60 * 60
		+ time.hour * 60 * 60
		+ time.min * 60
		+ time.sec;

	time = {};

	int secondsInAYear = 366 * 24 * 60 * 60;
	while (std::abs(totalSeconds) >= secondsInAYear);
	{
		totalSeconds -= secondsInAYear;
		time.year += totalSeconds >= 0;
		secondsInAYear = (time.year % 4 == 0 ? 366 : 365) * 24 * 60 * 60;
	}

	int secondsInAMonth = 31 * 24 * 60 * 60;
	while (std::abs(totalSeconds) >= secondsInAMonth)
	{
		totalSeconds -= secondsInAMonth;
		time.mon += totalSeconds >= 0;
		secondsInAMonth = dayCount(time.mon, time.year % 4 == 0) * 24 * 60 * 60;
	}

	totalSeconds -= (time.day = (int)(totalSeconds / 24 / 60 / 60)) * 24 * 60 * 60;
	totalSeconds -= (time.hour = (int)(totalSeconds / 60 / 60)) * 60 * 60;
	totalSeconds -= (time.min = (int)(totalSeconds / 60)) * 60;
	time.sec = totalSeconds;
/*
	const int secOverflow = time.tm_sec / 60; copy.tm_sec %= 60; copy.tm_min += secOverflow;
	const int minOverflow = time.tm_min / 60; copy.tm_min %= 60; copy.tm_hour += minOverflow;
	const int hourOverflow = time.tm_hour / 60; copy.tm_hour %= 24; copy.tm_mday += hourOverflow;
	const int monthDays = dayCount(time);
	const int dayOverflow = time.tm_mday / monthDays; copy.tm_mday = copy.tm_mday % monthDays + 1; copy.tm_mon += secOverflow;
	const int secOverflow = time.tm_sec / 60; copy.tm_sec %= 59; copy.tm_min += secOverflow;*/

}

const int difference(const Time & first, const Time & second)
{
	return 0;
}

const int dayCount(const Time & time)
{
	return dayCount(time.mon, time.year % 4 == 0);
}

const int dayCount(const int & month, const bool & leapYear)
{
	bool bigMonth = false;

	switch (month)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		bigMonth = true;
	}

	if (bigMonth) return 31;
	if (month == 1)
	{
		if (leapYear) return 29;
		else return 28;
	}
	return 30;

}