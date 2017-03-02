#pragma once

#include <stdio.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <map>
#include <vector>
#include <locale>
#include <algorithm>

typedef std::map<std::string, std::string> StrMap;
typedef std::vector<std::string> StrVec;

template <typename T>
T EnumFromString(const StrVec& enumStr, const char* value)
{
	int i = 0;

	function<string(string)> lower = [](string s)
	{
		std::transform(s.begin(), s.end(), s.begin(), ::tolower);

		return s;
	};

	for (auto it : enumStr)
	{
		if (lower(it) != lower(value))
		{
			++i;
			continue;
		}

		return (T)i;
	}

	throw std::runtime_error(string("Unknown value ") + value);
}