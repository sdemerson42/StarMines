#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <typeindex>

#include "Components.h"

#include "Entity.h"

class ComponentManager
{
	friend class Factory;
	friend class Renderer;
	friend class Animator;
	friend class Physics;
	friend class Behavior;
	friend class Particle;
	friend class GameState;
public:
	~ComponentManager()
	{
		std::cout << "Deleting ComponentManager...\n";
	}
	void addComponent(Entity *e, const std::string &tag, const std::vector<std::string> &initArgs);
	void deactivateComponent(Entity *e, const std::string &tag);
	void activateComponent(Entity *e, const std::string &tag);
	void removeComponent(Entity *e, const std::string &tag);
	void activateAll(Entity *e);
	void deactivateAll(Entity *e);
	void removeAll(Entity *e);
private:	
	static constexpr int MAX_COMPONENTS = 50000;
	
	std::vector<RenderComponent> m_render{ MAX_COMPONENTS };
	int m_renderSz{ 0 };
	std::vector<AnimationComponent> m_anim{ MAX_COMPONENTS };
	int m_animSz{ 0 };
	std::vector<PhysicsComponent> m_physics{ MAX_COMPONENTS };
	int m_physicsSz{ 0 };
	std::vector<BehaviorComponent> m_behavior{ MAX_COMPONENTS };
	int m_behaviorSz{ 0 };
	std::vector<TextComponent> m_text{ MAX_COMPONENTS };
	int m_textSz{ 0 };
	std::vector<ParticleComponent> m_particle{ MAX_COMPONENTS };
	int m_particleSz{ 0 };

	template<typename T>
	void genAddComponent(Entity *e, const std::vector<std::string> &initArgs, std::vector<T> &v, int &sz);
	template<typename T>
	void genDeactivateComponent(Entity *e, std::vector<T> &v, int &sz);
	template<typename T>
	void genActivateComponent(Entity *e, std::vector<T> &v, int &sz);
	template<typename T>
	void genRemoveComponent(Entity *e, std::vector<T> &v, int &sz);
};

template<typename T>
void ComponentManager::genAddComponent(Entity *e, const std::vector<std::string> &initArgs, std::vector<T> &v, int &sz)
{
	auto cp = std::find_if(begin(v), end(v), [&](auto &c)
	{
		return c.parent() == nullptr;
	});
	cp->initialize(initArgs);
	cp->setParent(e);
	e->m_compRef.push_back(&*cp);
	activateComponent(e, cp->getTag());
}
template<typename T>
void ComponentManager::genDeactivateComponent(Entity *e, std::vector<T> &v, int &sz)
{
	std::type_index ti{ typeid(T) };
	auto ecp = std::find_if(begin(e->m_compRef), end(e->m_compRef), [&](IComponent *p)
	{
		return std::type_index{ typeid(*p) } == ti;
	});

	if (ecp != end(e->m_compRef))
	{
		int i{ 0 };
		while (i < sz)
		{
			if (&v[i] == *ecp)
			{
				std::swap(v[i], v[sz - 1]);
				*ecp = &v[sz - 1];
				auto vip = v[i].parent();
				auto vipc = find(begin(vip->m_compRef), end(vip->m_compRef), &v[sz - 1]);
				*vipc = &v[i];

				/*if (ti == std::type_index{ typeid(BehaviorComponent) })
				{
					static_cast<BehaviorComponent *>(*vipc)->onMove();
					static_cast<BehaviorComponent *>(*ecp)->onMove();
				}*/

				--sz;
				break;
			}
			++i;
		}
	}
}


template<typename T>
void ComponentManager::genActivateComponent(Entity *e, std::vector<T> &v, int &sz)
{
	std::type_index ti{ typeid(T) };
	auto ecp = std::find_if(begin(e->m_compRef), end(e->m_compRef), [&](IComponent *p)
	{
		return std::type_index{ typeid(*p) } == ti;
	});

	if (ecp != end(e->m_compRef))
	{
		int i{ sz };
		while (i < MAX_COMPONENTS)
		{
			if (&v[i] == *ecp)
			{
				// Do not swap if we are already at the end of the vector...
				if (i == sz)
				{
					*ecp = &v[sz];
					(*ecp)->reactivate();
					++sz;
					break;
				}

				std::swap(v[i], v[sz]);
				*ecp = &v[sz];
				(*ecp)->reactivate();
				auto vip = v[i].parent();
				if (vip)
				{
					auto vipc = find(begin(vip->m_compRef), end(vip->m_compRef), &v[sz]);
					*vipc = &v[i];
					/*if (ti == std::type_index{ typeid(BehaviorComponent) })
					{
						static_cast<BehaviorComponent *>(*vipc)->onMove();
						static_cast<BehaviorComponent *>(*ecp)->onMove();
					}*/

				}
				++sz;
				break;
			}
			++i;
		}
	}
}

template<typename T>
void ComponentManager::genRemoveComponent(Entity *e, std::vector<T> &v, int &sz)
{
	genDeactivateComponent<T>(e, v, sz);
	std::type_index ti{ typeid(T) };
	auto ecp = std::find_if(begin(e->m_compRef), end(e->m_compRef), [&](IComponent *p)
	{
		return std::type_index{ typeid(*p) } == ti;
	});

	if (ecp != end(e->m_compRef))
	{
		(*ecp)->setParent(nullptr);
		e->m_compRef.erase(ecp);
	}

		
};