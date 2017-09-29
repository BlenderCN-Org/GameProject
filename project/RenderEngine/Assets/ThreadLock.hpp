#ifndef THREAD_LOCK_HPP
#define THREAD_LOCK_HPP

#include <cstdint>

class ThreadLock {

public:

	ThreadLock();
	~ThreadLock();

	bool lock();
	void unlock();

	bool hasLock() const;

private:

	uint32_t lockThreadId;

};

#endif