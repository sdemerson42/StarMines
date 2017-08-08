#pragma once

#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

// AutoList - Abstract class to allow access to all
// extant instances of a given type. Provides iterators
// to vector of raw pointers via begin() and end().
template<typename T>
class AutoList
{
protected:
	AutoList() noexcept
	{
		m_refs.emplace_back(static_cast<T*>(this));
	}
public:
	virtual ~AutoList() noexcept
	{
		auto p = find(std::begin(m_refs), std::end(m_refs), this);
		if (p != std::end(m_refs))
			m_refs.erase(p);
	}
	static T* get(int i)
	{
		return m_refs[i];
	}
	static unsigned size()
	{
		return m_ref.size();
	}
private:
	static vector<T*> m_refs;
};

template<typename T>
vector<T*> AutoList<T>::m_refs;