#pragma once

#include <vector>
#include <string>
#include "Serializable.h"

class IComponent : public Serializable
{
public:
	virtual ~IComponent()
	{}
	virtual void initialize(const std::vector<std::string> &input)
	{}
};
