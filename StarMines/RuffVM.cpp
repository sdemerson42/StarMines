#include "RuffVM.h"
#include "RuffParse.h"
#include "BehaviorComponent.h"
#include "AnimationComponent.h"
#include "PhysicsComponent.h"
#include "Entity.h"


#include <iostream>

void Ruff::RuffVM::loadScript(const std::string &fName)
{
	m_code = Ruff::parse(fName);
}

void Ruff::RuffVM::update()
{
	bool callAct{ false };
	if (m_parent->m_call.size() > 0)
	{
		for (auto &c : m_parent->m_call)
		{
			auto lb = m_code.label.find(c.label);
			if (lb == end(m_code.label))
				continue;
			callAct = true;
			m_parent->m_curCaller = c.caller;
			exec(lb->second);
		}
		m_parent->m_call.clear();
		m_parent->m_curCaller = nullptr;
	}

	if (!m_sleep && !callAct)
		exec();
	
}

void Ruff::RuffVM::exec(int line)
{
	
	int i{ 0 };

	if (line != -1)
	{
		i = line;
	}
	else if (m_pauseIndex != -1)
	{
		i = m_pauseIndex;
		m_pauseIndex = -1;
	}

	int letVar{ -1 };
	m_stack.clear();

	bool breakFlag{ false };
	bool subLabelFlag{ false };

	while(i < m_code.code.size())
	{
		switch (m_code.code[i])
		{
		case Code::add:
		{
			int r = pop();
			int l = pop();
			push(l+r);
			break;
		}
		case Code::dec:
		{
			int val = pop();
			pop();
			m_reg[letVar] -= val;
			letVar = -1;
			break;
		}
		case Code::div:
		{
			int r = pop();
			int l = pop();
			push(l / r);
			break;
		}
		case Code::ifEquals:
		{
			int dest = pop();
			int r = pop();
			int l = pop();
			if (l == r)
			{
				if (subLabelFlag)
				{
					subLabelFlag = false;
					m_frame.push_back(i + 1);
				}
				i = dest - 1;
			}
			letVar = -1;
			break;
		}
		case Code::ifGreater:
		{
			int dest = pop();
			int r = pop();
			int l = pop();
			if (l > r)
			{
				if (subLabelFlag)
				{
					subLabelFlag = false;
					m_frame.push_back(i + 1);
				}
				i = dest - 1;
			}
			letVar = -1;
			break;
		}
		case Code::ifLess:
		{
			int dest = pop();
			int r = pop();
			int l = pop();
			if (l < r)
			{
				if (subLabelFlag)
				{
					subLabelFlag = false;
					m_frame.push_back(i + 1);
				}
				i = dest - 1;
			}
			letVar = -1;
			break;
		}
		case Code::ifCallerTag:
		{
			int dest = pop();
			std::string tag{ strPop() };
			if (m_parent->m_curCaller && m_parent->m_curCaller->findTag(tag))
			{
				if (subLabelFlag)
				{
					subLabelFlag = false;
					m_frame.push_back(i + 1);
				}
				i = dest - 1;
			}
			letVar = -1;
			break;
		}
		case Code::inc:
		{
			int val = pop();
			pop();
			m_reg[letVar] += val;
			letVar = -1;
			break;
		}
		case Code::jump:
		{
			if (subLabelFlag)
			{
				subLabelFlag = false;
				m_frame.push_back(i + 1);
			}
			int dest = pop();
			i = dest - 1;
			letVar = -1;
			break;
		}
		case Code::label:
		{
			push(m_code.code[++i]);
			break;
		}
		case Code::sublabel:
		{
			push(m_code.code[++i]);
			subLabelFlag = true;
			break;
		}
		case Code::let:
		{
			int val = pop();
			pop();
			m_reg[letVar] = val;
			letVar = -1;
			break;
		}
		case Code::log:
		{
			int l{ 0 };
			if (letVar == -1)
				l = pop();
			else
				l = m_reg[letVar];
			std::cout << "Log: " << l << "\n";
			letVar = -1;
			break;
		}
		case Code::mod:
		{
			int r = pop();
			int l = pop();
			push(l % r);
			break;
		}
		case Code::mult:
		{
			int r = pop();
			int l = pop();
			push(l * r);
			break;
		}
		case Code::prim:
		{
			push(m_code.code[++i]);
			break;
		}
		case Code::sub:
		{
			int r = pop();
			int l = pop();
			push(l - r);
			break;
		}
		case Code::var:
		{
			++i;
			if (letVar == -1)
				letVar = m_code.code[i];
			push(m_reg[m_code.code[i]]);
			break;
		}
		case Code::halt:
		{
			breakFlag = true;
			letVar = -1;
			break;
		}
		case Code::rand:
		{
			int max = pop();
			int min = pop();
			pop();
			m_reg[letVar] = std::rand() % (max - min + 1) + min;
			letVar = -1;
			break;
		}
		case Code::retSub:
		{
			i = framePop() - 1;
			letVar = -1;
			break;
		}
		case Code::pause:
		{
			m_pauseIndex = i + 1;
			breakFlag = true;
			letVar = -1;
			break;
		}
		case Code::strbegin:
		{
			int sz{ 0 };
			while (m_code.code[++i] != Code::strend)
			{
				push(m_code.code[i]);
				++sz;
			}
			push(sz);
			break;
		}
		case Code::logStr:
		{
			auto s = strPop();
			std::cout << "Log: " << s << "\n";
			letVar = -1;
			break;
		}
		case Code::sendCall:
		{
			Call c;
			c.caller = m_parent->parent();
			c.label = strPop();
			
			auto p = m_parent->m_curCaller->getComponent<BehaviorComponent>();
			if (p)
				p->addCall(c);

			letVar = -1;
			break;
		}
		case Code::sendTag:
		{
			Call c;
			c.caller = m_parent->parent();
			c.label = strPop();
			std::string tag{ strPop() };
			m_parent->broadcastCall(c, tag);
			letVar = -1;
			break;
		}
		case Code::setDir:
		{
			float y = float(pop()) *0.1f;
			float x = float(pop()) *0.1f;

			auto p = m_parent->parent()->getComponent<PhysicsComponent>();
			if (p)
				p->setDir(x, y);

			letVar = -1;
			break;
		}
		case Code::sleep:
		{
			m_sleep = true;
			breakFlag = true;
			letVar = -1;
			break;
		}
		case Code::playAnim:
		{
			auto p = m_parent->parent()->getComponent<AnimationComponent>();
			if (p)
			{
				p->play(strPop());
			}
			letVar = -1;
			break;
		}
		case Code::setPos:
		{
			float y = float(pop());
			float x = float(pop());
			m_parent->parent()->setPosition(x, y);
			letVar = -1;
			break;
		}
		case Code::addPos:
		{
			float y = float(pop());
			float x = float(pop());
			m_parent->parent()->addPosition(x, y);
			letVar = -1;
			break;
		}
		case Code::spawn:
		{
			float y = float(pop());
			float x = float(pop());
			std::string tag{ strPop() };
			Events::SpawnDataEvent sde{ tag, x, y };
			m_parent->broadcast(&sde);
			letVar = -1;
			break;
		}
		case Code::despawn:
		{
			Events::DespawnEvent de{ m_parent->parent() };
			m_parent->broadcast(&de);
			letVar = -1;
			break;
		}
		case Code::setSpeed:
		{
			float spd = float(pop()) * 0.1f;
			auto p = m_parent->parent()->getComponent<PhysicsComponent>();
			if (p)
				p->setSpeed(spd);
			letVar = -1;
			break;
		}
		case Code::letSpeed:
		{
			pop();
			float spd{ 0.0f };
			auto p = m_parent->parent()->getComponent<PhysicsComponent>();
			if (p)
				spd = p->speed() * 10.0f;
			m_reg[letVar] = int(spd);
			letVar = -1;
			break;
		};
		case Code::letDirX:
		{
			pop();
			float x{ 0.0f };
			auto p = m_parent->parent()->getComponent<PhysicsComponent>();
			if (p)
				x = p->dir().x * 10.0f;
			m_reg[letVar] = int(x);
			letVar = -1;
			break;
		};
		case Code::letDirY:
		{
			pop();
			float y{ 0.0f };
			auto p = m_parent->parent()->getComponent<PhysicsComponent>();
			if (p)
				y = p->dir().y * 10.0f;
			m_reg[letVar] = int(y);
			letVar = -1;
			break;
		};
		case Code::letPosX:
		{
			pop();
			float x = m_parent->parent()->position().x;
			m_reg[letVar] = int(x);
			letVar = -1;
			break;
		};
		case Code::letPosY:
		{
			pop();
			float y = m_parent->parent()->position().y;
			m_reg[letVar] = int(y);
			letVar = -1;
			break;
		};

		}

		if (breakFlag) break;

		++i;
	}
}

void Ruff::RuffVM::setParent(BehaviorComponent *parent)
{
	m_parent = parent;
}

void Ruff::RuffVM::reset()
{
	m_stack.clear();
	m_frame.clear();
	m_pauseIndex = -1;
	m_sleep = false;
}

int Ruff::RuffVM::pop()
{
	int r = *(end(m_stack) - 1);
	m_stack.pop_back();
	return r;
}

void Ruff::RuffVM::push(int x)
{
	m_stack.emplace_back(x);
}

int Ruff::RuffVM::framePop()
{
	int r = *(end(m_frame) - 1);
	m_frame.pop_back();
	return r;
}

void Ruff::RuffVM::framePush(int x)
{
	m_frame.emplace_back(x);
}

std::string Ruff::RuffVM::strPop()
{
	int sz = pop();
	std::string s;
	for (int i = 0; i < sz; ++i)
		s.push_back(pop());
	return s;
}