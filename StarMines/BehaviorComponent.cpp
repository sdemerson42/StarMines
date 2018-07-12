#include "BehaviorComponent.h"
#include "PhysicsComponent.h"
#include "AnimationComponent.h"
#include "TextComponent.h"
#include "RenderComponent.h"
#include "Entity.h"
#include "Events.h"

std::string BehaviorComponent::m_tag{ "behavior" };
Events::InputEvent BehaviorComponent::m_input{};
const float BehaviorComponent::m_axisDeadzone{ 20.0f };
BehaviorComponent *BehaviorComponent::m_currentComponent{ nullptr };
//std::map<std::string, Ruff::ByteCode> BehaviorComponent::m_codeMap;

//void BehaviorComponent::resetVM()
//{
//	m_vm.reset();
//}

void BehaviorComponent::broadcastCall(Ruff::Call &c, const std::string &tag)
{
	Events::RSCallEvent rsc;
	rsc.call = c;
	rsc.tag = tag;
	broadcast(&rsc);
}

void BehaviorComponent::onQueryEntityByTag(const Events::QueryEntityByTagEvent *evnt)
{
	if (evnt->client == this)
		m_target = evnt->response;
}


// ==================================  Lua API definitions

const std::string &BehaviorComponent::module()
{
	return m_luaModule;
}
const Vector2 &BehaviorComponent::position()
{
	return parent()->position();
}
void BehaviorComponent::setPosition(float x, float y)
{
	parent()->setPosition(x, y);
}
float BehaviorComponent::speed()
{
	auto c = parent()->getComponent<PhysicsComponent>();
	if (c)
		return c->speed();
}
void BehaviorComponent::setSpeed(float x)
{
	auto c = parent()->getComponent<PhysicsComponent>();
	if (c)
		c->setSpeed(x);
}
const Vector2 &BehaviorComponent::dir()
{
	auto c = parent()->getComponent<PhysicsComponent>();
	if (c)
		return c->dir();
}
void BehaviorComponent::setDir(float x, float y)
{
	auto c = parent()->getComponent<PhysicsComponent>();
	if (c)
		c->setDir(x, y);
}

const Ruff::Call &BehaviorComponent::getCall()
{

	Ruff::Call &r = m_curCall;

	if (m_call.size() == 0 || !active())
	{
		r.label = "nil";
		r.caller = nullptr;
		if (!active())
			m_call.clear();

		return r;
	}
	r = m_call[0];

	callDataSync(r);

	m_call.erase(begin(m_call));
	return r;
}

void BehaviorComponent::clearCalls()
{
	m_call.clear();
}

void BehaviorComponent::sendToTag
(const std::string &tag, const std::string &label, const std::string &sdata = {})
{
	Ruff::Call c;
	c.caller = this->parent();
	c.tag = c.caller->name();
	c.label = label;
	c.sdata = sdata;
	c.bc = this;
	callDataSync(c);
	broadcastCall(c, tag);
}

void BehaviorComponent::sendToCaller(const std::string &label, const std::string &sdata = {})
{
	auto e = m_curCall.caller;
	if (e)
	{
		auto b = e->getComponent<BehaviorComponent>();
		if (b)
		{
			Ruff::Call c;
			c.caller = this->parent();
			c.tag = c.caller->name();
			c.label = label;
			c.sdata = sdata;
			c.bc = this;
			callDataSync(c);
			b->addPendingCall(c);
		}
	}
}

void BehaviorComponent::spawn(const std::string &bTag, float x, float y, const std::string &sdata,
	const std::string &persist = "default")
{
	Events::SpawnDataEvent sde{ bTag, x, y };
	sde.persist = persist;
	sde.sInitData = sdata;
	sde.bc = this;
	spawnDataSync(sde);
	broadcast(&sde);
}

void BehaviorComponent::despawn(const std::string &sdata)
{
	auto &sdData = refSceneDespawnData();
	sdData.clear();
	
	if (sdata.size() > 0)
	{
		std::string n;
		for (char ch : sdata)
		{
			if (ch == ',')
			{
				sdData.push_back(std::stoi(n));
				n = "";
			}
			else
			{
				n += ch;
			}
		}
		sdData.push_back(std::stoi(n));
	}

	Events::DespawnEvent de{ parent() };
	broadcast(&de);
}

void BehaviorComponent::playSound(const std::string &tag, float volume, bool hi, bool loop)
{
	Events::SoundEvent se{ tag, loop, hi, false, volume };
	se.tag = tag;
	se.volume = volume;
	se.hiPriority = hi;
	se.loop = loop;
	broadcast(&se);
}
void BehaviorComponent::stopSound(const std::string &tag)
{
	Events::SoundEvent se;
	se.tag = tag;
	se.stop = true;
	broadcast(&se);
}

void BehaviorComponent::playMusic(const std::string &tag, float volume, bool loop)
{
	Events::MusicEvent me{ tag, loop, false, volume };
	broadcast(&me);
}

void BehaviorComponent::stopMusic()
{
	Events::MusicEvent me;
	me.stop = true;
	broadcast(&me);
}

void BehaviorComponent::playAnim(const std::string &tag)
{
	auto a = parent()->getComponent<AnimationComponent>();
	if (a)
		a->play(tag);
}

void BehaviorComponent::setTargetByCaller()
{
	m_target = m_curCall.caller;
}
void BehaviorComponent::setTargetByTag(const std::string &tag, const std::string &method)
{
	Events::QueryEntityByTagEvent q{ this, tag, method };
	broadcast(&q);
}
const Vector2 &BehaviorComponent::targetPosition()
{
	if (m_target)
		return m_target->position();
	Vector2 r;
	r.x = -1.0f;
	r.y = -1.0f;
	return r;
}
BehaviorComponent *BehaviorComponent::targetBehavior()
{
	if (m_target)
	{
		auto bc = m_target->getComponent<BehaviorComponent>();
		if (bc)
			return bc;
	}
	return nullptr;
}

void BehaviorComponent::deactivate()
{
	setActive(false);
}

bool BehaviorComponent::active()
{
	return IComponent::active();
}

const Events::InputEvent &BehaviorComponent::input() const
{
	return m_input;
}
void BehaviorComponent::setText(const std::string &txt)
{
	auto c = parent()->getComponent<TextComponent>();
	if (c)
		c->setString(txt);
}

void BehaviorComponent::appendText(const std::string &txt)
{
	auto c = parent()->getComponent<TextComponent>();
	if (c)
		c->appendString(txt);
}

void BehaviorComponent::newScene(const std::string &scene)
{
	Events::SceneChangeEvent sce;
	sce.name = scene;
	broadcast(&sce);
}

bool BehaviorComponent::globalPersist()
{
	return parent()->persist() == Entity::PersistType::Global;
}

void BehaviorComponent::createNewScene(const std::string &name, const std::string &prox, const std::string &view)
{
	Events::CreateSceneFromScriptEvent e;
	e.name = name;

	std::vector<int> proxVal;
	std::vector<float> viewVal;

	// Parse Lua strings into prox and view data

	std::string n;

	for (char c : prox)
	{
		if (c == ',')
		{
			proxVal.push_back(stoi(n));
			n = "";
		}
		else
		{
			n += c;
		}
	}
	proxVal.push_back(stoi(n));
	n = "";

	for (char c : view)
	{
		if (c == ',')
		{
			viewVal.push_back(stof(n));
			n = "";
		}
		else
		{
			n += c;
		}
	}
	viewVal.push_back(stof(n));

	e.prox.sceneX = proxVal[0];
	e.prox.sceneY = proxVal[1];
	e.prox.cellX = proxVal[2];
	e.prox.cellY = proxVal[3];;
	e.view.viewW = viewVal[0];
	e.view.viewH = viewVal[1];
	e.view.portX = viewVal[2];
	e.view.portY = viewVal[3];
	e.view.portW = viewVal[4];
	e.view.portH = viewVal[5];
	e.view.centerX = viewVal[6];
	e.view.centerY = viewVal[7];

	broadcast(&e);
}

void BehaviorComponent::addSceneData(const std::string &name, const std::string &persist, bool cache, int count,
	const std::string &blueprint, float x, float y, const std::string &sInitData)
{
	Events::AddSceneDataEvent e;
	e.name = name;
	e.persist = persist;
	e.cache = cache;
	e.count = count;
	e.blueprint = blueprint;
	e.x = x;
	e.y = y;
	e.init = sInitData;

	broadcast(&e);
}

void BehaviorComponent::rotate(float angle)
{
	auto c = parent()->getComponent<RenderComponent>();
	if (c)
		c->rotate(angle);
}

void BehaviorComponent::scale(float x, float y)
{
	auto c = parent()->getComponent<RenderComponent>();
	if (c)
		c->setScale(x, y);
}

const RenderComponent::Color &BehaviorComponent::color()
{
	auto c = parent()->getComponent<RenderComponent>();
	if (c)
		return c->color();
}

void BehaviorComponent::setColor(int r, int g, int b, int a)
{
	auto c = parent()->getComponent<RenderComponent>();
	if (c)
		c->setColor(r, g, b, a);
}

const RenderComponent::Color &BehaviorComponent::textColor()
{
	auto c = parent()->getComponent<TextComponent>();
	if (c)
		return c->color();
}

void BehaviorComponent::setTextColor(int r, int g, int b, int a)
{
	auto c = parent()->getComponent<TextComponent>();
	if (c)
		c->setColor(r, g, b, a);
}

bool BehaviorComponent::callerHasTag(const std::string &tag)
{
	if (m_curCall.caller)
		return m_curCall.caller->findTag(tag);
	return false;
}

// ==================================== END LUA ========================================================

void BehaviorComponent::callDataSync(Ruff::Call &c)
{
	if (c.data.size() > 0)
	{
		c.sdata = "";
		for (auto x : c.data)
		{
			c.sdata += std::to_string(x);
			c.sdata += ",";
		}
	}
	else if (c.sdata.size() > 0)
	{
		std::string n;
		for (char ch : c.sdata)
		{
			if (ch == ',')
			{
				c.data.push_back(std::stoi(n));
				n = "";
			}
			else
			{
				n += ch;
			}
		}
		c.data.push_back(std::stoi(n));
	}
}

void BehaviorComponent::spawnDataSync(Events::SpawnDataEvent &e)
{
	if ( e.initData.size() > 0)
	{
		e.sInitData = "";
		for (auto x :  e.initData)
		{
			e.sInitData += std::to_string(x);
			e.sInitData += ",";
		}
	}
	else if (e.sInitData.size() > 0)
	{
		std::string n;
		for (char ch : e.sInitData)
		{
			if (ch == ',')
			{
				 e.initData.push_back(std::stoi(n));
				n = "";
			}
			else
			{
				n += ch;
			}
		}
		 e.initData.push_back(std::stoi(n));
	}
}


// Initialization

//extern "C" _declspec(dllexport) BehaviorComponent *Behavior_getCurrentComponent()
//{
//	return BehaviorComponent::getCurrentComponent();
//}
//
//// Register values
//
//extern "C" _declspec(dllexport) void Behavior_setRegisterInt(BehaviorComponent *bc, int index, int val)
//{
//	bc->setRegisterInt(index, val);
//}
//
//extern "C" _declspec(dllexport) int Behavior_getRegisterInt(BehaviorComponent *bc, int index)
//{
//	return bc->getRegisterInt(index);
//}
//
//extern "C" _declspec(dllexport) void Behavior_setRegisterFloat(BehaviorComponent *bc, int index, float val)
//{
//	bc->setRegisterFloat(index, val);
//}
//
//extern "C" _declspec(dllexport) float Behavior_getRegisterFloat(BehaviorComponent *bc, int index)
//{
//	return bc->getRegisterFloat(index);
//}
//
//// Deactivation
//
//extern "C" _declspec(dllexport) void Behavior_deactivate(BehaviorComponent *bc)
//{
//	bc->setActive(false);
//}
//
//// Spawn / despawn messaging
//
//extern "C" _declspec(dllexport) void Behavior_spawn(BehaviorComponent *bc, const char *b, float x, float y, int data[], int dataSz)
//{
//	Events::SpawnDataEvent sde{ b, x, y };
//	for (int i = 0; i < dataSz; ++i)
//		sde.initData.push_back(data[i]);
//	bc->broadcast(&sde);
//}
//
//extern "C" _declspec(dllexport) void Behavior_despawn(BehaviorComponent *bc, int data[], int dataSz)
//{
//	auto &sdData = bc->refSceneDespawnData();
//	sdData.clear();
//	for (int i = 0; i < dataSz; ++i)
//		sdData.push_back(data[i]);
//
//	Events::DespawnEvent de{ bc->parent() };
//	bc->broadcast(&de);
//}
//
//// Position
//
//extern "C" _declspec(dllexport) const Vector2 *Behavior_position(BehaviorComponent *bc)
//{
//	return &bc->parent()->position();
//}
//
//extern "C" _declspec(dllexport) void Behavior_setPosition(BehaviorComponent *bc, float x, float y)
//{
//	bc->parent()->setPosition(x, y);
//}
//
//// Sending and interpreting Calls
//
//extern "C" _declspec(dllexport) Ruff::CCall *Behavior_getCall(BehaviorComponent *bc)
//{
//	auto &calls = bc->getCalls();
//
//	if (calls.size() == bc->ccDelCount) return nullptr;
//	
//	auto i = bc->ccDelCount;
//	bc->curCCall.caller = calls[i].caller;
//	bc->curCCall.label = calls[i].label.c_str();
//	if (calls[i].data.size() > 0)
//		bc->curCCall.data = &calls[i].data[0];
//	else
//		bc->curCCall.data = nullptr;
//	bc->curCCall.sz = calls[i].data.size();
//
//	++bc->ccDelCount;
//	return &bc->curCCall;
//}
//
//extern "C" _declspec(dllexport) void Behavior_sendToCaller(BehaviorComponent *bc, Entity *receiver, const char *message, 
//	int data[], int dataSz)
//{
//	if (receiver)
//	{
//		auto p = receiver->getComponent<BehaviorComponent>();
//		if (p)
//		{
//			Ruff::Call c;
//			c.caller = bc->parent();
//			c.label = message;
//			for (int i = 0; i < dataSz; ++i)
//				c.data.emplace_back(data[i]);
//			p->addCall(c);
//		}
//	}
//}
//
//extern "C" _declspec(dllexport) void Behavior_sendToTag(BehaviorComponent *bc, const char *tag, const char *message, 
//	int data[], int dataSz)
//{
//	Ruff::Call c;
//	c.caller = bc->parent();
//	c.label = message;
//	std::string ltag{ tag };
//	for (int i = 0; i < dataSz; ++i)
//		c.data.emplace_back(data[i]);
//	bc->broadcastCall(c, ltag);
//}
//
//// Physics
//
//extern "C" _declspec(dllexport) void Behavior_setDirection(BehaviorComponent *bc, float x, float y)
//{
//	auto ph = bc->parent()->getComponent<PhysicsComponent>();
//	if (ph)
//		ph->setDir(x, y);
//}
//
//extern "C" _declspec(dllexport) Vector2 *Behavior_direction(BehaviorComponent *bc)
//{
//	auto ph = bc->parent()->getComponent<PhysicsComponent>();
//	if (ph)
//		return &ph->dir();
//}
//
//extern "C" _declspec(dllexport) void Behavior_setSpeed(BehaviorComponent *bc, float sp)
//{
//	auto ph = bc->parent()->getComponent<PhysicsComponent>();
//	if (ph)
//		ph->setSpeed(sp);
//}
//
//extern "C" _declspec(dllexport) float Behavior_speed(BehaviorComponent *bc)
//{
//	auto ph = bc->parent()->getComponent<PhysicsComponent>();
//	if (ph)
//		return ph->speed();
//}
//
//// Animation
//
//extern "C" _declspec(dllexport) void Behavior_playAnimation(BehaviorComponent *bc, const char *anim)
//{
//	auto a = bc->parent()->getComponent<AnimationComponent>();
//	if (a)
//		a->play(anim);
//}
//
//// Targeting
//
//extern "C" _declspec(dllexport) void Behavior_setTargetBySender(BehaviorComponent *bc, Entity *t)
//{
//	bc->target() = t;
//}
//
//extern "C" _declspec(dllexport) void Behavior_setTargetByTag(BehaviorComponent *bc, const char *tag, const char *searchMethod)
//{
//	bc->setTargetTag(tag, searchMethod);
//}
//
//extern "C" _declspec(dllexport) const Vector2 *Behavior_targetPosition(BehaviorComponent *bc)
//{
//	if (bc->target() != nullptr)
//		return &bc->target()->position();
//	return nullptr;
//}
//
//// Input
//
//extern "C" _declspec(dllexport) const BehaviorComponent::CInput *Behavior_input(BehaviorComponent *bc)
//{
//	return &bc->input();
//}
//
//// Sound
//
//extern "C" _declspec(dllexport) void Behavior_playSound(BehaviorComponent *bc, const char *tag, float volume, bool hi, 
//	bool loop)
//{
//	Events::SoundEvent se{ tag, loop, hi, false, volume };
//	bc->broadcast(&se);
//}
//
//extern "C" _declspec(dllexport) void Behavior_stopSound(BehaviorComponent *bc, const char *tag)
//{
//	Events::SoundEvent se;
//	se.stop = true;
//	se.tag = tag;
//	bc->broadcast(&se);
//}
//
//// Text
//
//extern "C" _declspec(dllexport) void Behavior_setText(BehaviorComponent *bc, const char *s)
//{
//	auto p = bc->parent()->getComponent<TextComponent>();
//	if (p)
//		p->setString(s);
//}
//
//extern "C" _declspec(dllexport) void Behavior_appendText(BehaviorComponent *bc, const char *s)
//{
//	auto p = bc->parent()->getComponent<TextComponent>();
//	if (p)
//		p->appendString(s);
//}
//
//// Scene
//
//extern "C" _declspec(dllexport) void Behavior_newScene(BehaviorComponent *bc, const char *scene)
//{
//	Events::SceneChangeEvent sce;
//	sce.name = scene;
//	bc->broadcast(&sce);
//}
//
//extern "C" _declspec(dllexport) void Behavior_sendSceneSpawnData(BehaviorComponent *bc, int data[], int sz)
//{
//	auto &sdd = bc->getSceneDespawnData();
//	for (int i = 0; i < sz; ++i)
//		sdd.push_back(data[i]);
//	sdd.clear();
//}
//
//// Parent Entity (for comparisons)
//
//extern "C" _declspec(dllexport) Entity *Behavior_parent(BehaviorComponent *bc)
//{
//	return bc->parent();
//}