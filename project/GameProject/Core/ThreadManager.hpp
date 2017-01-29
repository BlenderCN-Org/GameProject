#ifndef THREADMANAGER_HPP
#define THREADMANAGER_HPP

// project includes
#include "SafeQueue.hpp"
#include "task.hpp"

// std includes
#include <thread>

// system dependent include

class ThreadManager
{
public:

	static ThreadManager* getThreadManager();
	static void release();

	void startThreads(uint32_t nrThreads);
	void stopThreads();

	void queueTask(Task &task);
	Task getNextTast();

private:

	static ThreadManager* threadManager;

	uint32_t threadCount;
	std::thread* workerThreads;

	SafeQueue<Task> taskQueue;

	void threadProcess(uint32_t threadIndex);

	void loadAsset(AssetLoadStruct &ls);
};

#endif