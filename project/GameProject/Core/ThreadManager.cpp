// project includes
#include "ThreadManager.hpp"

// std includes
#include <chrono>
#include <assert.h>

void ThreadManager::startThreads(uint32_t nrThreads) {
	threadCount = nrThreads;

	workerThreads = new std::thread[threadCount];

	for ( uint32_t i = 0; i < threadCount; i++ ) {
		workerThreads[i] = std::thread(&ThreadManager::threadProcess, this, i);
	}
}

void ThreadManager::stopThreads() {
	Task t;
	t.type = TaskType::eTaskType_threadTerminate;

	queueTask(t);

	for ( uint32_t i = 0; i < threadCount; i++ ) {
		workerThreads[i].join();
		printf("Thread %d closed\n", i);
	}

	delete[] workerThreads;
}

void ThreadManager::queueTask(Task & task) {
	taskQueue.enqueue(task);
}

Task ThreadManager::getNextTast() {
	return taskQueue.dequeue();
}

void ThreadManager::threadProcess(uint32_t threadIndex) {
	bool running = true;

	while ( running ) {
		printf("Thread %d waiting for task\n", threadIndex);
		Task t = getNextTast();
		printf("Thread %d processing a task\n", threadIndex);

		switch ( t.type ) {
			case TaskType::eTaskType_threadTerminate:
				running = false;
				queueTask(t);
				break;
			case TaskType::eTaskType_loadAsset:
				AssetLoadStruct ls = *(AssetLoadStruct*)(t.data);
				loadAsset(ls);
				break;
			default:
				break;
		}
	}
}