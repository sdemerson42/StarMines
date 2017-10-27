#pragma once

#include "Events.h"
#include <map>
#include <memory>
#include <typeindex>
#include <vector>


	template<typename T, typename ET>
	using MFunc = void(T::*)(const ET *);


	class IFuncWrapper
	{
	public:
		virtual void call(const EventBase *) = 0;
	};

	template<typename T, typename ET>
	class FuncWrapper : public IFuncWrapper
	{
	public:
		FuncWrapper(T *caller, MFunc<T, ET> func) :
			m_caller{ caller }, m_func{ func }
		{}
		void call(const EventBase *evnt) override
		{
			(m_caller->*m_func)(static_cast<const ET*>(evnt));
		}
	private:
		T *m_caller;
		MFunc<T, ET> m_func;
	};

	class EventHandler
	{
	public:
		virtual ~EventHandler();
		template<typename T, typename ET>
		void registerFunc(T *caller, MFunc<T, ET> func)
		{
			std::type_index ti{ typeid(ET) };
			m_funcMap[ti] = std::make_shared<FuncWrapper<T, ET>>(caller, func);
			m_receiverMap[ti].emplace_back(caller);
		}
		void handleEvent(const EventBase *evnt);
		void broadcast(const EventBase *evnt);
	private:
		std::map<std::type_index, std::shared_ptr<IFuncWrapper>> m_funcMap;
		static std::map<std::type_index, std::vector<EventHandler *>> m_receiverMap;
	};

