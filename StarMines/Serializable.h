#pragma once

#include <utility>
#include <vector>
#include <string>

class Serializable
{
	using Data = std::vector<std::string>;
public:
	void readSerial(const Data &d);
	void writeSerial(Data &d);
protected:
	Serializable() = default;
	enum class Type
	{
		INT, FLOAT, BOOL, STRING
	};
	template<typename ...Args>
	void initSerial(void *member, Args ...args)
	{
		m_memberTable.clear();
		addMembers(member, args...);
	}
private:
	template<typename ...Args>
	void addMembers(void *member, Type type, Args ...args)
	{
		m_memberTable.emplace_back(std::make_pair<void *, Type>(std::move(member), std::move(type)));
		addMembers(args...);
	}
	void addMembers()
	{}
	std::vector<std::pair<void *, Type>> m_memberTable;
	bool stob(const std::string &s);
};
