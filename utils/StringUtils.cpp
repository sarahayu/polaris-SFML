#include "StringUtils.h"
#include <algorithm>

const std::vector<std::string> su::split(const std::string &str, const char &delim)
{
	std::stringstream ss(str);
	std::string item;
	std::vector<std::string> tokens;
	while (std::getline(ss, item, delim))
	{
		tokens.push_back(item);
	}
	return tokens;

}

const bool su::isDigit(const std::string &str)
{
	std::string buffer = str;
	bool neg = false;

	if (buffer.length() == 0) return false;
	else if (buffer[0] == '-') neg = true;
	else if (!isdigit(buffer[0])) return false;

	if (neg)
	{
		std::string remainding = buffer.substr(1, buffer.length());
		if (remainding.length() == 0 || !isdigit(remainding[0])) return false;
	}

	return true;
}

const int su::fast_strToD(const char * str)
{
	int val = 0;
	while (*str) {
		val = val * 10 + (*str++ - '0');
	}
	return val;
}

const int su::strToD(const std::string &str)
{/*
	std::string buffer = str;
	bool neg = false;

	if (buffer.length() == 0) buffer = "0";
	else if (buffer[0] == '-') neg = true;
	else if (!isdigit(buffer[0])) buffer = "0";

	int number = 0;

	if (neg)
	{
		std::string remainding = buffer.substr(1, buffer.length());
		if (remainding.length() == 0 || !isdigit(remainding[0])) number = 0;
		else number = -std::stoi(remainding);
	}
	else number = std::stoi(buffer);*/
	std::stringstream s; s << str;
	int i; s >> i;

	return i;
}

const bool su::isFloat(const std::string &str)
{
	std::string buffer = str;
	bool neg = false;

	if (buffer.length() == 0) return false;
	else if (buffer[0] == '-') neg = true;
	else if (!isdigit(buffer[0])) return false;

	if (neg)
	{
		std::string remainding = buffer.substr(1, buffer.length());
		if (remainding.length() == 0 || !isdigit(remainding[0])) return false;
	}

	return true;
}

const float su::strToF(const std::string & str)
{
	std::string buffer = str;
	bool neg = false;

	if (buffer.length() == 0) buffer = "0";
	else if (buffer[0] == '-') neg = true;
	else if (!isdigit(buffer[0])) buffer = "0";

	float number = 0.0f;

	if (neg)
	{
		std::string remainding = buffer.substr(1, buffer.length());
		if (remainding.length() == 0 || !isdigit(remainding[0])) number = 0.0f;
		else number = -std::stof(remainding);
	}
	else number = std::stof(buffer);

	return number;
}

const bool su::isChar(const int &unicode)
{
	switch (unicode)
	{
	case 39:	// apostrophe
	case 44:	// comma
	case 45:	// dash
	case 46:	// period
	case 47:	// forward slash
	case 58:	// colon
		return true;
	}

	return (unicode == 32							// spacebar
		|| (unicode >= 48 && unicode <= 57)			// numbers
		|| (unicode >= 65 && unicode <= 90)			// lowercase letters
		|| (unicode >= 97 && unicode <= 122));		// uppercase letters
}

const std::string su::toString(const tm &ttime)
{
	int min = ttime.tm_min;
	std::string strMin = std::to_string(min);
	int sec = ttime.tm_sec;
	std::string strSec = std::to_string(sec);

	return std::to_string(ttime.tm_hour) + ":" + (min < 10 ? "0" + strMin : strMin) + ":" + (sec < 10 ? "0" + strSec : strSec);
}

const std::string su::toString(const float &hours)
{
	const float min = (hours - (int)hours) * 60.f;
	const float sec = (min - (int)min) * 60.f;
	return std::to_string((int)hours) + ":" + std::to_string((int)min) + ":" + std::to_string((int)sec);
}

const std::string su::toLower(const std::string & str)
{
	std::string newStr;
	newStr.resize(str.length());

	std::transform(str.begin(), str.end(), newStr.begin(), ::tolower);

	return newStr;
}
