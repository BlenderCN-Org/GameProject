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

	CEThread(ThreadManager& tm);
	virtual ~CEThread();

	void stopThread();

	void threadLoop();

private:

	bool running;
	std::thread* thrd;

	ThreadManager& threadManager;

};

class ThreadManager {

public:


	Task nextTask();

private:

	SafeQueue<Task> threadTasks;


};


#endif