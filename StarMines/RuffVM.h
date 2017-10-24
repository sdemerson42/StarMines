#pragma once

#include "RuffCommon.h"
#include <vector>

namespace Ruff
{
	using ByteCode = std::vector<int>;

	class RuffVM
	{
	public:
		RuffVM() :
			m_pauseIndex{ -1 }
		{
			m_reg.resize(100);
			m_code.reserve(500);
			m_stack.reserve(20);
			m_frame.reserve(20);
		}
		void loadScript(const std::string &fName);
		void exec();
	private:
		int pop();
		void push(int x);
		int framePop();
		void framePush(int x);
		
		std::vector<int> m_stack;
		std::vector<int> m_reg;
		std::vector<int> m_frame;
		int m_pauseIndex;

		ByteCode m_code;
	};
}