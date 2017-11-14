#pragma once

#include "RuffCommon.h"
#include <vector>

class BehaviorComponent;

namespace Ruff
{

	class RuffVM
	{
	public:
		RuffVM(BehaviorComponent *parent) :
			m_parent{ parent }, m_pauseIndex{ -1 }, m_sleep{ false }
		{
			m_reg.resize(100);
			m_stack.reserve(20);
			m_frame.reserve(20);
		}
		void loadScript(const std::string &fName);
		void update();
		void exec(int line = -1);
		void setParent(BehaviorComponent *parent);
		void reset();
	private:
		BehaviorComponent *m_parent;
		int pop();
		void push(int x);
		int framePop();
		void framePush(int x);
		std::string strPop();
		std::vector<int> m_stack;
		std::vector<int> m_reg;
		std::vector<int> m_frame;
		std::vector<int> m_letVar;
		int m_pauseIndex;
		bool m_sleep;

		ByteCode m_code;
	};
}