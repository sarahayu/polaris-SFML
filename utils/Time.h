#pragma once

struct Time
{
	int sec;   // seconds after the minute - [0, 60] including leap second
	int min;   // minutes after the hour - [0, 59]
	int hour;  // hours since midnight - [0, 23]
	int day;  // day of the month - [1, 31]
	int mon;   // months since January - [0, 11]
	int year;  // years in AD
};

void maketime(Time &time);
const int difference(const Time &first, const Time &second);
const int dayCount(const Time &time);
const int dayCount(const int &month, const bool &leapYear);   // months since January - [0, 11]
