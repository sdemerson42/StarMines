#pragma once
#include <Windows.h>

/* Concur.h

	Small header-only library to add basic concurrency functionality to the project.
	Will consist primarily of wrapper classes and helper functions to make the winAPI
	kernel objects easier to work with.
*/

namespace Concur
{
	// Handle - Class for managing HANDLEs
	class Handle
	{
	public:
		Handle(HANDLE handle = nullptr) noexcept :
			m_handle{ handle }
		{}
		~Handle() noexcept
		{
			close();
		}
		Handle(const Handle &) = delete;
		Handle &operator=(const Handle &) = delete;
		Handle(Handle &&other) noexcept:
		m_handle{ other.release() }
		{}
		Handle &operator=(Handle &&other) noexcept
		{
			if (this != &other)
			{
				reset(other.release());
			}
			return *this;
		}

		explicit operator bool() const
		{
			return m_handle != nullptr;
		}
		void close()
		{
			if (*this)
				CloseHandle(m_handle);
		}
		void reset(HANDLE h = nullptr)
		{
			close();
			m_handle = h;
		}
		HANDLE release()
		{
			HANDLE r = m_handle;
			m_handle = nullptr;
			return r;
		}
		HANDLE get() const
		{
			return m_handle;
		}
	private:
		HANDLE m_handle;
	};

	// callback - __stdcall function used by makeThread. Should not be called directly.
	DWORD __stdcall callback(HANDLE func)
	{
		static_cast<void(*)()>(func)();
		return 0;
	}

	// makeThread - Helper function for thread creation. Returns a Handle
	// containing a HANDLE to a thread kernel object. Takes a pointer to
	// a function returning void and taking no arguments.
	Handle makeThread(void(*func)())
	{
		return Handle
		{
			CreateThread(nullptr, 0, callback, func, 0, nullptr)
		};
	}

	// packHandles - Function that fills a dynamic array of HANDLEs.
	// Used by waitMultiple and should not be called directly.
	template<typename ...Args>
	void packHandles(HANDLE *left, const Handle &right, const Args &...args)
	{
		*left = right.get();
		packHandles(++left, args...);
	}

	void packHandles(HANDLE *)
	{}

	// waitMultiple - Helper function that halts execution on the current
	// thread until argument threads have exited. Takes any number of
	// Handle references.
	template<typename ...Args>
	void waitMultiple(const Args &...args)
	{
		constexpr unsigned sz = sizeof...(Args);
		HANDLE p[sz];
		packHandles(p, args...);
		WaitForMultipleObjects(sz, p, true, INFINITE);
	}

	// LockWrapper - Class that releases a referenced lock on destruction. Not meant
	// to be instanced directly; returned by SlimLock get() and getShared() functions.
	template<typename T>
	class LockWrapper
	{
		using Action = void(T::*)();
	public:
		LockWrapper(T &lock, Action action) noexcept :
			m_lock{ lock }, m_action{ action }
		{}
		~LockWrapper() noexcept
		{
			if (m_action)
				(m_lock.*m_action)();
		}
		LockWrapper(const LockWrapper &) = delete;
		LockWrapper &operator=(const LockWrapper &) = delete;
		LockWrapper(LockWrapper &&other) noexcept :
		LockWrapper(other.m_lock, other.m_action)
		{
			other.m_action = nullptr;
		}
		LockWrapper &operator=(LockWrapper &&other) noexcept
		{
			m_lock = other.m_lock;
			m_action = other.m_action;
			other.m_action = nullptr;
		}
	private:
		T &m_lock;
		Action m_action;
	};

	// SlimLock - Wrapper class for SRWLOCKs. Can provide automatic lock release by use of get() and getShared(),
	// which return LockWrappers that will release the lock on destruction.
	class SlimLock
	{
	public:
		SlimLock() = default;
		~SlimLock() = default;
		SlimLock(const SlimLock &) = delete;
		SlimLock(SlimLock &&) = delete;
		SlimLock &operator=(const SlimLock &) = delete;
		SlimLock &operator=(SlimLock &&) = delete;

		void enter()
		{
			AcquireSRWLockExclusive(&m_lock);
		}
		void enterShared()
		{
			AcquireSRWLockShared(&m_lock);
		}
		void exit()
		{
			ReleaseSRWLockExclusive(&m_lock);
		}
		void exitShared()
		{
			ReleaseSRWLockShared(&m_lock);
		}
		// Returns a LockWrapper that will release exclusive lock on destruction.
		LockWrapper<SlimLock> get()
		{
			enter();
			return LockWrapper<SlimLock>{*this, &SlimLock::exit};
		}
		// Returns a LockWrapper that will release shared lock on destruction.
		LockWrapper<SlimLock> getShared()
		{
			enterShared();
			return LockWrapper<SlimLock>{*this, &SlimLock::exitShared};
		}
	private:
		SRWLOCK m_lock{ SRWLOCK_INIT };
	};

}