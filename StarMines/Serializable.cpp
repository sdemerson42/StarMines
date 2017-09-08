#include "Serializable.h"

void Serializable::readSerial(const Data &d)
{
	auto i = begin(m_memberTable);
	for (auto &s : d)
	{
		auto t = i->second;
		auto p = i->first;
		switch (t)
		{
		case Type::INT:
			*static_cast<int*>(p) = stoi(s);
			break;
		case Type::FLOAT:
			*static_cast<float*>(p) = stof(s);
			break;
		case Type::BOOL:
			*static_cast<bool*>(p) = stob(s);
			break;
		case Type::STRING:
			*static_cast<std::string*>(p) = s;
			break;
		};
		++i;
	}
}

void Serializable::writeSerial(Data &d)
{
	for (auto &pr : m_memberTable)
	{
		auto t = pr.second;
		auto p = pr.first;
		switch (t)
		{
		case Type::INT:
			d.emplace_back(std::to_string(*static_cast<int*>(p)));
			break;
		case Type::FLOAT:
			d.emplace_back(std::to_string(*static_cast<float*>(p)));
			break;
		case Type::BOOL:
		{
			std::string s;
			bool b = *static_cast<bool*>(p);
			s = b ? "true" : "false";
			d.emplace_back(s);
		}
		break;
		case Type::STRING:
			d.emplace_back(*static_cast<std::string*>(p));
			break;
		};
	}
}

bool Serializable::stob(const std::string &s)
{
	if (s == "true") return true;
	return false;
}