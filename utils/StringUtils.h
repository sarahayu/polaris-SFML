#pragma once
#include <sstream>
#include <iostream>
#include <vector>

namespace su
{
	const std::vector<std::string> split(const std::string &str, const char &delim);

	const bool isDigit(const std::string &str);
	const int fast_strToD(const char * str);
	const int strToD(const std::string &str);

	const bool isFloat(const std::string &str);

	const float strToF(const std::string & str);

	const bool isChar(const int &unicode);

	const std::string toString(const tm &ttime);

	const std::string toString(const float &hours);

	const std::string toLower(const std::string &str);
}