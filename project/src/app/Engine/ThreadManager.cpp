
/// Internal Includes
#include "ThreadManager.hpp"

/// External Includes

/// Std Includes
#include <memory>
#include <chrono>

CEThread::CEThread(ThreadManager& tm, uint32_t _id)
	: threadManager(tm)
	, id(_id) 
	, procCounts(0U)
	, lastTaskTime(0.0F)
{

	running.store(true);
	thrd = new std::thread(&CEThread::threadLoop, this);

}

CEThread::~CEThread() {
	thrd->join();
	delete thrd;
}

void CEThread::stopThread() {
	printf("Stopping thread with id %d\n", id);
	running.store(false);
}

void CEThread::threadLoop() {

	procCounts = 0;

	while (running.load()) {

		//std::this_thread::sleep_for(std::chrono::milliseconds(10));
		ITask* t = threadManager.nextTask();

		clock.tick();

		t->execute();
		procCounts++;
		clock.tick();

		lastTaskTime = clock.seconds();

		delete t;
	}

	printf("Thread %d done!\n", id);

}

ThreadManager::ThreadManager()
	: threadTasks()
	, threads(nullptr) {

	int coreCount = Engine::System::getLogicalProcessorCount();

	threads = (CEThread*)malloc(coreCount * sizeof(CEThread));
	if (nullptr != threads) {
		printf("Creating %d threads\n", coreCount);

		for (int i = 0; i < coreCount; i++) {
			void* ptr = &threads[i];
			new (ptr) CEThread(*this, i);
		}
	}
}

ThreadManager::~ThreadManager() {

	int coreCount = Engine::System::getLogicalProcessorCount();

	printf("Waitng for queue to become empty\n");

	while (!threadTasks.empty());

	printf("Stopping %d threads\n", coreCount);

	for (int i = 0; i < coreCount; i++) {
		threads[i].stopThread();
	}

	//std::this_thread::sleep_for(std::chrono::milliseconds(10));

	for (int i = 0; i < coreCount; i++) {
		QuitTask* qt = new QuitTask();
		queueTask(qt);
	}

	for (int i = 0; i < coreCount; i++) {
		threads[i].~CEThread();
	}

	free(threads);
}

ITask* ThreadManager::nextTask() {
	return threadTasks.dequeue();
}

std::string ThreadManager::getThreadInfo(int index) {
	int coreCount = Engine::System::getLogicalProcessorCount();
	std::string str = "";

	if (index < coreCount) {
		str += "T " + std::to_string(index) + " p " + std::to_string(threads[index].procCounts) + " " + std::to_string(threads[index].lastTaskTime);
	}

	return str;
}
