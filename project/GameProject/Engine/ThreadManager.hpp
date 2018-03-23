#ifndef THREAD_MANAGER_HPP
#define THREAD_MANAGER_HPP

/// Internal Includes
#include "SafeQueue.hpp"

/// External Includes

/// Std Includes
#include <thread>

class ThreadManager;

class Task {

};

class CEThread {

public:

	CEThread(ThreadManager& tm, uint32_t _id);
	virtual ~CEThread();

	void stopThread();

	void threadLoop();

private:

	uint32_t id;
	bool running;
	std::thread* thrd;

	ThreadManager& threadManager;

};

class ThreadManager {

public:

	ThreadManager();
	virtual ~ThreadManager();

	Task nextTask();

private:

	SafeQueue<Task> threadTasks;

	CEThread* threads;

};


#endif