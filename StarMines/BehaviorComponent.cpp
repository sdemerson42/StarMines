#include "BehaviorComponent.h"
#include "PhysicsComponent.h"
#include "AnimationComponent.h"
#include "TextComponent.h"
#include "Entity.h"
#include "Events.h"

std::string BehaviorComponent::m_tag{ "behavior" };
BehaviorComponent::CInput BehaviorComponent::m_input{};
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

void BehaviorComponent::setTargetTag(const std::string &tag, const std::string &method)
{
	Events::QueryEntityByTagEvent q{ this, tag, method };
	broadcast(&q);
}
void BehaviorComponent::onQueryEntityByTag(const Events::QueryEntityByTagEvent *evnt)
{
	if (evnt->client == this)
		m_target = evnt->response;
}

// C Python API

// Initialization

extern "C" _declspec(dllexport) BehaviorComponent *Behavior_getCurrentComponent()
{
	return BehaviorComponent::getCurrentComponent();
}

// Register values

extern "C" _declspec(dllexport) void Behavior_setRegisterInt(BehaviorComponent *bc, int index, int val)
{
	bc->setRegisterInt(index, val);
}

extern "C" _declspec(dllexport) int Behavior_getRegisterInt(BehaviorComponent *bc, int index)
{
	return bc->getRegisterInt(index);
}

extern "C" _declspec(dllexport) void Behavior_setRegisterFloat(BehaviorComponent *bc, int index, float val)
{
	bc->setRegisterFloat(index, val);
}

extern "C" _declspec(dllexport) float Behavior_getRegisterFloat(BehaviorComponent *bc, int index)
{
	return bc->getRegisterFloat(index);
}

// Deactivation

extern "C" _declspec(dllexport) void Behavior_deactivate(BehaviorComponent *bc)
{
	bc->setActive(false);
}

// Spawn / despawn messaging

extern "C" _declspec(dllexport) void Behavior_spawn(BehaviorComponent *bc, const char *b, float x, float y, int data[], int dataSz)
{
	Events::SpawnDataEvent sde{ b, x, y };
	for (int i = 0; i < dataSz; ++i)
		sde.initData.push_back(data[i]);
	bc->broadcast(&sde);
}

extern "C" _declspec(dllexport) void Behavior_despawn(BehaviorComponent *bc, int data[], int dataSz)
{
	auto &sdData = bc->refSceneDespawnData();
	sdData.clear();
	for (int i = 0; i < dataSz; ++i)
		sdData.push_back(data[i]);

	Events::DespawnEvent de{ bc->parent() };
	bc->broadcast(&de);
}

// Position

extern "C" _declspec(dllexport) const Vector2 *Behavior_position(BehaviorComponent *bc)
{
	return &bc->parent()->position();
}

extern "C" _declspec(dllexport) void Behavior_setPosition(BehaviorComponent *bc, float x, float y)
{
	bc->parent()->setPosition(x, y);
}

// Sending and interpreting Calls

extern "C" _declspec(dllexport) Ruff::CCall *Behavior_getCall(BehaviorComponent *bc)
{
	auto &calls = bc->getCalls();

	if (calls.size() == bc->ccDelCount) return nullptr;
	
	auto i = bc->ccDelCount;
	bc->curCCall.caller = calls[i].caller;
	bc->curCCall.label = calls[i].label.c_str();
	if (calls[i].data.size() > 0)
		bc->curCCall.data = &calls[i].data[0];
	else
		bc->curCCall.data = nullptr;
	bc->curCCall.sz = calls[i].data.size();

	++bc->ccDelCount;
	return &bc->curCCall;
}

extern "C" _declspec(dllexport) void Behavior_sendToCaller(BehaviorComponent *bc, Entity *receiver, const char *message, 
	int data[], int dataSz)
{
	if (receiver)
	{
		auto p = receiver->getComponent<BehaviorComponent>();
		if (p)
		{
			Ruff::Call c;
			c.caller = bc->parent();
			c.label = message;
			for (int i = 0; i < dataSz; ++i)
				c.data.emplace_back(data[i]);
			p->addCall(c);
		}
	}
}

extern "C" _declspec(dllexport) void Behavior_sendToTag(BehaviorComponent *bc, const char *tag, const char *message, 
	int data[], int dataSz)
{
	Ruff::Call c;
	c.caller = bc->parent();
	c.label = message;
	std::string ltag{ tag };
	for (int i = 0; i < dataSz; ++i)
		c.data.emplace_back(data[i]);
	bc->broadcastCall(c, ltag);
}

// Physics

extern "C" _declspec(dllexport) void Behavior_setDirection(BehaviorComponent *bc, float x, float y)
{
	auto ph = bc->parent()->getComponent<PhysicsComponent>();
	if (ph)
		ph->setDir(x, y);
}

extern "C" _declspec(dllexport) Vector2 *Behavior_direction(BehaviorComponent *bc)
{
	auto ph = bc->parent()->getComponent<PhysicsComponent>();
	if (ph)
		return &ph->dir();
}

extern "C" _declspec(dllexport) void Behavior_setSpeed(BehaviorComponent *bc, float sp)
{
	auto ph = bc->parent()->getComponent<PhysicsComponent>();
	if (ph)
		ph->setSpeed(sp);
}

extern "C" _declspec(dllexport) float Behavior_speed(BehaviorComponent *bc)
{
	auto ph = bc->parent()->getComponent<PhysicsComponent>();
	if (ph)
		return ph->speed();
}

// Animation

extern "C" _declspec(dllexport) void Behavior_playAnimation(BehaviorComponent *bc, const char *anim)
{
	auto a = bc->parent()->getComponent<AnimationComponent>();
	if (a)
		a->play(anim);
}

// Targeting

extern "C" _declspec(dllexport) void Behavior_setTargetBySender(BehaviorComponent *bc, Entity *t)
{
	bc->target() = t;
}

extern "C" _declspec(dllexport) void Behavior_setTargetByTag(BehaviorComponent *bc, const char *tag, const char *searchMethod)
{
	bc->setTargetTag(tag, searchMethod);
}

extern "C" _declspec(dllexport) const Vector2 *Behavior_targetPosition(BehaviorComponent *bc)
{
	if (bc->target() != nullptr)
		return &bc->target()->position();
	return nullptr;
}

// Input

extern "C" _declspec(dllexport) const BehaviorComponent::CInput *Behavior_input(BehaviorComponent *bc)
{
	return &bc->input();
}

// Sound

extern "C" _declspec(dllexport) void Behavior_playSound(BehaviorComponent *bc, const char *tag, float volume, bool hi, 
	bool loop)
{
	Events::SoundEvent se{ tag, loop, hi, false, volume };
	bc->broadcast(&se);
}

extern "C" _declspec(dllexport) void Behavior_stopSound(BehaviorComponent *bc, const char *tag)
{
	Events::SoundEvent se;
	se.stop = true;
	se.tag = tag;
	bc->broadcast(&se);
}

// Text

extern "C" _declspec(dllexport) void Behavior_setText(BehaviorComponent *bc, const char *s)
{
	auto p = bc->parent()->getComponent<TextComponent>();
	if (p)
		p->setString(s);
}

extern "C" _declspec(dllexport) void Behavior_appendText(BehaviorComponent *bc, const char *s)
{
	auto p = bc->parent()->getComponent<TextComponent>();
	if (p)
		p->appendString(s);
}

// Scene

extern "C" _declspec(dllexport) void Behavior_newScene(BehaviorComponent *bc, const char *scene)
{
	Events::SceneChangeEvent sce;
	sce.name = scene;
	bc->broadcast(&sce);
}

extern "C" _declspec(dllexport) void Behavior_sendSceneSpawnData(BehaviorComponent *bc, int data[], int sz)
{
	auto &sdd = bc->getSceneDespawnData();
	for (int i = 0; i < sz; ++i)
		sdd.push_back(data[i]);
	sdd.clear();
}

// Parent Entity (for comparisons)

extern "C" _declspec(dllexport) Entity *Behavior_parent(BehaviorComponent *bc)
{
	return bc->parent();
}