#include "ThreadLock.hpp"
#include "../ReGlobal.hpp"

ThreadLock::ThreadLock() {
	lockThreadId = std::thread::id();
}

ThreadLock::~ThreadLock() {

}

bool ThreadLock::lock() {

	bool aquired = false;

	// if 0 nobody has lock
	if (lockThreadId == std::thread::id()) {
		lockThreadId = getThreadId();
		aquired = true;
	}
	// owner calling lock again, just return true
	else if (lockThreadId == getThreadId()) {
		aquired = true;
	}

	return aquired;
}

void ThreadLock::unlock() {

	// only unlock if current thread calls unlock
	if (lockThreadId == getThreadId()) {
		lockThreadId = std::thread::id();
	}

}

bool ThreadLock::hasLock() const {
	
	bool hasLock = false;
	
	// returns true if current thread has the lock
	if (lockThreadId == getThreadId()) {
		hasLock = true;
	}
	return hasLock;
}