#include "ReadData.h"

std::istream &readData::operator >> (std::istream &ist, std::vector<std::string> &v)
{
	std::string s;
	
	while (ist >> s && s != "*")
		v.emplace_back(s);

	return ist;
}

std::vector<std::string> readData::loadStringData(const std::string &fName)
{
	std::ifstream ifs{ fName };
	std::vector<std::string> r;
	
	ifs >> r;
	ifs.close();
	
	return r;
}

std::vector<std::vector<std::string>> readData::loadMultipleStringData(const std::string &fName)
{
	std::ifstream ifs{ fName };
	std::vector<std::vector<std::string>> r;

	while (ifs)
	{
		std::vector<std::string> vs;
		ifs >> vs;
		r.emplace_back(vs);
	}

	ifs.close();
	return r;
}