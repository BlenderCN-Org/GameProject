// project includes
#include "ThreadManager.hpp"
#include "Loading\OBJLoader.hpp"
#include "Assets\ModelAsset.hpp"

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

void ThreadManager::loadAsset(AssetLoadStruct &ls) {

	uint32_t dataSize = 0;
	void* data = nullptr;

	switch ( ls.howToLoad.type) {
		case LoadType::eLoadType_file:
		{
			data = ObjLoader::load(ls.howToLoad.fileName, dataSize);
			ModelAsset* ma = (ModelAsset*)ls.asset;
			ma->setDataPtrAndSize(data, dataSize);
			delete data;
			ma->setAssetState(AssetState::eAssetState_loaded);
		}
			break;
		case LoadType::eLoadType_memory:
			assert(0 && "memory data copy not implemented");
			break;
		case LoadType::eLoadType_network:
			assert(0 && "Network asset loading not implemented");
			break;
		default:
			assert(0 && "Undefined loading type");
			break;
	}
	

}
