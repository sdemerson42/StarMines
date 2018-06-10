#include "BehaviorComponent.h"
#include "PhysicsComponent.h"
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

// Position

//TEST

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

	if (bc->callIndex == calls.size()) return nullptr;

	bc->curCCall.caller = calls[0].caller;
	bc->curCCall.label = calls[0].label.c_str();
	bc->curCCall.data = &calls[0].data[0];
	bc->curCCall.sz = calls[0].data.size();
	++bc->callIndex;

	return &bc->curCCall;
}

//extern "C" _declspec(dllexport) void Behavior_sendToCaller(BehaviorComponent *bc, const char *message, 
//	int data[], int dataSz)
//{
//	auto caller = bc->getCaller();
//	if (caller)
//	{
//		auto p = caller->getComponent<BehaviorComponent>();
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
