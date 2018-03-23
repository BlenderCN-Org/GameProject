#ifndef THREAD_LOCK_HPP
#define THREAD_LOCK_HPP

#include <cstdint>
#include <thread>

class ThreadLock {

public:

	ThreadLock();
	~ThreadLock();

	bool lock();
	void unlock();

	bool hasLock() const;

private:

	std::thread::id lockThreadId;

};

#endif