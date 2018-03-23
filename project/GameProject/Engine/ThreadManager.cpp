#include "ThreadManager.hpp"
#include "Core/System.hpp"

#include <memory>
#include <chrono>

CEThread::CEThread(ThreadManager& tm, uint32_t _id) : threadManager(tm) , id(_id) {

	running = true;
	thrd = new std::thread(&CEThread::threadLoop, this);

}

CEThread::~CEThread() {
	stopThread();
}


void CEThread::stopThread() {
	printf("Stopping thread with id %d\n", id);
	running = false;
	thrd->join();
	delete thrd;
}

void CEThread::threadLoop() {

	while (running) {

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		//Task t = tm.

	}

}

ThreadManager::ThreadManager() {

	int coreCount = Engine::System::getLogicalProcessorCount();

	threads = (CEThread*)malloc(coreCount * sizeof(CEThread));

	printf("Creating %d threads\n", coreCount);

	for (int i = 0; i < coreCount; i++) {
		void* ptr = &threads[i];
		new (ptr) CEThread(*this, i);
	}

}

ThreadManager::~ThreadManager() {

	int coreCount = Engine::System::getLogicalProcessorCount();

	printf("Stopping %d threads\n", coreCount);

	for (int i = 0; i < coreCount; i++) {
		threads[i].~CEThread();
	}
	free(threads);
}

Task ThreadManager::nextTask() {
	return threadTasks.dequeue();
}