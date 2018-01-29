#include "ThreadManager.hpp"

CEThread::CEThread(ThreadManager& tm) : threadManager(tm) {

	//thrd = new std::thread(this, &CEThread::threadLoop);

}

CEThread::~CEThread() {
	stopThread();
}


void CEThread::stopThread() {
	running = false;
	thrd->join();
}

void CEThread::threadLoop() {

	while (running) {

		//Task t = tm.

	}

}


Task ThreadManager::nextTask() {
	return threadTasks.dequeue();
}