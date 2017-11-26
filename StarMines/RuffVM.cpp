#include "RuffVM.h"
#include "RuffParse.h"
#include "Components.h"
#include "Entity.h"


#include <iostream>

void Ruff::RuffVM::loadScript(const std::string &fName)
{
	m_code.code.clear();
	m_code.label.clear();
	m_code = Ruff::parse(fName);
}

void Ruff::RuffVM::update()
{
	if (m_parent->m_call.size() > 0)
	{
			
		for (auto &c : m_parent->m_call)
		{
			if (!m_lock)
			{
				auto lb = m_code.label.find(c.label);
				if (lb == end(m_code.label))
					continue;
				m_sleep = false;
				m_parent->m_curCaller = c.caller;
				m_callData = &c.data;
				m_pauseIndex = -1;
				exec(lb->second);
			}
		}
		m_parent->m_call.clear();
		m_parent->m_curCaller = nullptr;
	}
	if (!m_sleep)
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
			m_reg[m_letVar[0]] -= val;
			m_letVar.clear();
			break;
		}
		case Code::div:
		{
			int r = pop();
			int l = pop();
			push(l / r);
			break;
		}
		case Code::_greater:
		{
			m_compOperator.push_back('>');
			break;
		}
		case Code::_less:
		{
			m_compOperator.push_back('<');
			break;
		}
		case Code::_equal:
		{
			m_compOperator.push_back('=');
			break;
		}
		case Code::_notEqual:
		{
			m_compOperator.push_back('!');
			break;
		}
		case Code::_and:
		{
			m_compCombine.push_back('&');
			break;
		}
		case Code::_or:
		{
			m_compCombine.push_back('|');
			break;
		}
		case Code::_if:
		{
			int dest = pop();
			while (m_compOperator.size() > 0)
			{
				int r = pop();
				int l = pop();
				char comp = opPop();
				bool b = false;
				if (comp == '>' && l > r)
					b = true;
				else if (comp == '<' && l < r)
					b = true;
				else if (comp == '=' && l == r)
					b = true;
				else if (comp == '!' && l != r)
					b = true;
				m_truth.emplace_back(b);
			}
			int j = m_truth.size() - 1;
			bool b{ m_truth[j] };
			for (auto &c : m_compCombine)
			{
				--j;
				bool bb{ m_truth[j] };
				if (c == '&')
				{
					if (b && bb) b = true;
					else b = false;
				}
				if (c == '|')
				{
					if (b || bb) b = true;
					else b = false;
				}
			}
			if (b)
			{
				if (subLabelFlag)
				{
					subLabelFlag = false;
					m_frame.push_back(i + 1);
				}
				i = dest - 1;
			}
			m_compCombine.clear();
			m_compOperator.clear();
			m_truth.clear();
			m_letVar.clear();
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
			break;
		}
		case Code::inc:
		{
			int val = pop();
			pop();
			m_reg[m_letVar[0]] += val;
			m_letVar.clear();
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
			m_reg[m_letVar[0]] = val;
			m_letVar.clear();
			break;
		}
		case Code::log:
		{
			int l{ 0 };
			if (m_letVar.size() == 0)
				l = pop();
			else
				l = m_reg[m_letVar[0]];
			std::cout << "Log: " << l << "\n";
			m_letVar.clear();
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
			m_letVar.emplace_back(m_code.code[i]);
			push(m_reg[m_code.code[i]]);
			break;
		}
		case Code::halt:
		{
			breakFlag = true;
			break;
		}
		case Code::rand:
		{
			int max = pop();
			int min = pop();
			pop();
			m_reg[m_letVar[0]] = std::rand() % (max - min + 1) + min;
			m_letVar.clear();
			break;
		}
		case Code::retSub:
		{
			i = framePop() - 1;
			break;
		}
		case Code::pause:
		{
			m_pauseIndex = i + 1;
			breakFlag = true;
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
			break;
		}
		case Code::sendCall:
		{
			if (m_parent->m_curCaller)
			{
				auto p = m_parent->m_curCaller->getComponent<BehaviorComponent>();
				if (p)
				{
					Call c;
					c.caller = m_parent->parent();
					c.label = strPop();
					for (auto k : m_stack)
						c.data.emplace_back(k);
					m_stack.clear();
					p->addCall(c);
				}
			}
			m_letVar.clear();
			break;
		}
		case Code::sendTag:
		{
			Call c;
			c.caller = m_parent->parent();
			c.label = strPop();
			std::string tag{ strPop() };
			for (auto k : m_stack)
				c.data.emplace_back(k);
			m_stack.clear();
			m_parent->broadcastCall(c, tag);
			m_letVar.clear();
			break;
		}
		case Code::setDir:
		{
			float y = float(pop()) *0.1f;
			float x = float(pop()) *0.1f;

			auto p = m_parent->parent()->getComponent<PhysicsComponent>();
			if (p)
				p->setDir(x, y);

			m_letVar.clear();
			break;
		}
		case Code::sleep:
		{
			m_sleep = true;
			breakFlag = true;
			break;
		}
		case Code::playAnim:
		{
			auto p = m_parent->parent()->getComponent<AnimationComponent>();
			if (p)
			{
				p->play(strPop());
			}
			break;
		}
		case Code::setPos:
		{
			float y = float(pop());
			float x = float(pop());
			m_parent->parent()->setPosition(x, y);
			m_letVar.clear();
			break;
		}
		case Code::addPos:
		{
			float y = float(pop());
			float x = float(pop());
			m_parent->parent()->addPosition(x, y);
			m_letVar.clear();
			break;
		}
		case Code::spawn:
		{
			float y = float(pop());
			float x = float(pop());
			std::string tag{ strPop() };
			Events::SpawnDataEvent sde{ tag, x, y };
			for (auto k : m_stack)
				sde.initData.emplace_back(k);
			m_stack.clear();
			m_parent->broadcast(&sde);
			m_letVar.clear();
			break;
		}
		case Code::despawn:
		{
			m_parent->m_sceneDespawnData.clear();
			for (auto k : m_stack)
				m_parent->m_sceneDespawnData.emplace_back(k);
			m_stack.clear();
			Events::DespawnEvent de{ m_parent->parent() };
			m_parent->broadcast(&de);
			break;
		}
		case Code::setSpeed:
		{
			float spd = float(pop()) * 0.1f;
			auto p = m_parent->parent()->getComponent<PhysicsComponent>();
			if (p)
				p->setSpeed(spd);
			m_letVar.clear();
			break;
		}
		case Code::letSpeed:
		{
			pop();
			float spd{ 0.0f };
			auto p = m_parent->parent()->getComponent<PhysicsComponent>();
			if (p)
				spd = p->speed() * 10.0f;
			m_reg[m_letVar[0]] = int(spd);
			m_letVar.clear();
			break;
		}
		case Code::letPos:
		{
			pop();
			pop();
			float x = m_parent->parent()->position().x;
			float y = m_parent-> parent()->position().y;
			m_reg[m_letVar[0]] = int(x);
			m_reg[m_letVar[1]] = int(y);
			m_letVar.clear();
			break;
		}
		case Code::letDir:
		{
			pop();
			pop();
			auto pc = m_parent->parent()->getComponent<PhysicsComponent>();
			if (pc)
			{
				float x = pc->dir().x * 10.0f;
				float y = pc->dir().y * 10.0f;
				m_reg[m_letVar[0]] = int(x);
				m_reg[m_letVar[1]] = int(y);
			}
			m_letVar.clear();
			break;
		}
		case Code::posBound:
		{
			float yr = pop();
			float yl = pop();
			float xr = pop();
			float xl = pop();
			Vector2 pos = m_parent->parent()->position();

			if (pos.x < xl) pos.x = xl;
			if (pos.x > xr) pos.x = xr;
			if (pos.y < yl) pos.y = yl;
			if (pos.y > yr) pos.y = yr;
			m_parent->parent()->setPosition(pos.x, pos.y);

			m_letVar.clear();
			break;
		}
		case Code::setTargetCaller:
		{
			m_parent->m_target = m_parent->m_curCaller;
			break;
		}
		case Code::letTargetPos:
		{
			pop();
			pop();
			if (m_parent->m_target)
			{
				float x = m_parent->m_target->position().x;
				float y = m_parent->m_target->position().y;
				m_reg[m_letVar[0]] = int(x);
				m_reg[m_letVar[1]] = int(y);
			}
			m_letVar.clear();
			break;
		}
		case Code::setTargetTag:
		{
			std::string method = strPop();
			std::string tag = strPop();
			m_parent->setTargetTag(tag, method);
			break;
		}
		case Code::letInputAxis:
		{
			pop();
			pop();
			pop();
			pop();
			m_reg[m_letVar[0]] = int(m_parent->m_input.xAxis);
			m_reg[m_letVar[1]] = int(m_parent->m_input.yAxis);
			m_reg[m_letVar[2]] = int(m_parent->m_input.uAxis);
			m_reg[m_letVar[3]] = int(m_parent->m_input.vAxis);
			m_letVar.clear();
			break;
		}
		case Code::letCallData:
		{
			if (m_callData)
			{
				int dCount{ 0 };
				for (auto val : *m_callData)
				{
					m_reg[m_letVar[dCount++]] = val;
					pop();
				}
			}
			m_letVar.clear();
			break;
		}
		case Code::lock:
		{
			m_lock = true;
			break;
		}
		case Code::unlock:
		{
			m_lock = false;
			break;
		}
		case Code::playSound:
		{
			float volume = float(pop());
			bool hiPriority = (pop() == 1 ? true : false);
			bool loop = (pop() == 1 ? true : false);
			std::string tag{ strPop() };
			Events::SoundEvent se{ tag, loop, hiPriority, false, volume };
			m_parent->broadcast(&se);

			m_letVar.clear();
			break;
		}
		case Code::stopSound:
		{
			Events::SoundEvent se;
			se.stop = true;
			se.tag = strPop();
			m_parent->broadcast(&se);
			break;
		}
		case Code::setText:
		{
			auto c = m_parent->parent()->getComponent<TextComponent>();
			if (c)
				c->setString(strPop());
			break;
		}
		case Code::setTextVal:
		{
			auto c = m_parent->parent()->getComponent<TextComponent>();
			if (c)
				c->setString(std::to_string(pop()));
			m_letVar.clear();
			break;
		}
		case Code::appendText:
		{
			auto c = m_parent->parent()->getComponent<TextComponent>();
			if (c)
				c->appendString(strPop());
			break;
		}
		case Code::appendTextVal:
		{
			auto c = m_parent->parent()->getComponent<TextComponent>();
			if (c)
				c->appendString(std::to_string(pop()));
			m_letVar.clear();
			break;
		}
		case Code::sendSceneSpawnData:
		{
			m_parent->m_sceneDespawnData.clear();
			for (auto k : m_stack)
				m_parent->m_sceneDespawnData.emplace_back(k);
			m_stack.clear();
			m_letVar.clear();
			break;
		}
		case Code::newScene:
		{
			Events::SceneChangeEvent sce;
			sce.name = strPop();
			m_parent->broadcast(&sce);
			break;
		}

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
	m_reg.clear();
	m_reg.resize(20);
	m_parent->m_call.clear();
	m_parent->m_curCaller = nullptr;
	m_lock = false;

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

char Ruff::RuffVM::opPop()
{
	char r = *(end(m_compOperator) - 1);
	m_compOperator.pop_back();
	return r;
}

char Ruff::RuffVM::combPop()
{
	char r = *(end(m_compCombine) - 1);
	m_compCombine.pop_back();
	return r;
}
