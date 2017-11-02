#include "Factory.h"
#include "GameState.h"
#include "ComponentManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>

std::istream &operator >> (std::istream &is, Factory::Blueprint &b);


Factory::Factory(GameState *gameState, const std::string &fName) :
	m_gameState{ gameState }
{
	std::ifstream ifs{ fName };
	if (!ifs)
	{
		std::cerr << "WARNING: Factory failed to open file for initialization.\n";
	}

	while (true)
	{
		Blueprint b;
		if (!(ifs >> b))
			break;
		m_blueprint.emplace_back(b);
	}
}

std::istream &operator >> (std::istream &is, Factory::CompData &cd)
{
	std::string s;
	if (!(is >> s) || s != "[")
		return is;
	if (!(is >> s))
		return is;
	cd.type = s;

	while (true)
	{
		if (!(is >> s))
			return is;
		if (s == "]")
			break;
		cd.data.emplace_back(s);
	}

	return is;
}

std::istream &operator >> (std::istream &is, Factory::Blueprint &b)
{
	std::string s;
	if (!(is >> s))
		return is;

	b.name = s;
	char c;
	if (!(is >> c) || c != ':')
	{
		std::cerr << "WARNING: Factory failed to read Blueprint (formatting error).\n";
		return is;
	}
	
	while (true)
	{
		if (!(is >> s))
		{
			std::cerr << "WARNING: Factory failed to read Blueprint (formatting error).\n";
			return is;
		}
		if (s == "{")
			break;
		b.tag.emplace_back(s);
	}

	while (true)
	{
		if (!(is >> s))
		{
			std::cerr << "WARNING: Factory failed to read Blueprint (formatting error).\n";
			return is;
		}

		if (s == "}")
			break;
		if (s == "[")
			is.putback(s[0]);

		Factory::CompData cd;
		if (!(is >> cd))
		{
			std::cerr << "WARNING: Factory failed to read Blueprint (error reading Component data).\n";
			return is;
		}
		b.compData.emplace_back(cd);
	}

	return is;
}

void Factory::createFromBlueprint(const std::string &blueprint, float x, float y)
{
	auto p = std::find_if(begin(m_blueprint), end(m_blueprint), [&](const Blueprint &b)
	{
		return b.name == blueprint;
	});
	if (p == end(m_blueprint))
	{
		std::cerr << "WARNING: Factory failed to create Entity from Blueprint '" << blueprint << "'.\n";
		return;
	}
	m_gameState->m_entity.emplace_back(std::make_shared<Entity>());
	auto e = m_gameState->m_entity[m_gameState->m_entity.size()-1].get();
	e->setPosition(x, y);
	for (auto &t : p->tag)
		e->addTag(t);

	for (auto &cd : p->compData)
	{
		m_gameState->m_compManager->addComponent(e, cd.type, cd.data);
	}
}