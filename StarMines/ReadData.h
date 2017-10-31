#pragma once


#include <vector>
#include <string>
#include <fstream>

namespace readData
{
	std::istream &operator >> (std::istream &ist, std::vector<std::string> &v);

	std::vector<std::string> loadStringData(const std::string &fName);
	std::vector<std::vector<std::string>> loadMultipleStringData(const std::string &fName);
}
