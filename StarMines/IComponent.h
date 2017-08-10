#pragma once

#include <vector>
#include <string>

class IComponent
{
public:
	virtual ~IComponent()
	{}
	virtual void initialize(const std::vector<std::string> &input)
	{}
};
