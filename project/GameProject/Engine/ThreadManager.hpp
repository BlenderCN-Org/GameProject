#ifndef THREAD_MANAGER_HPP
#define THREAD_MANAGER_HPP

/// Internal Includes
#include "Core/System.hpp"
#include "SafeQueue.hpp"

/// External Includes
#include <PhysicsEngine/PhysicsEngine.hpp>

/// Std Includes
#include <thread>
#include <atomic>
#include <string>

class ThreadManager;

class ITask {
public:
	virtual ~ITask() {}
	virtual void execute() = 0;
};

class QuitTask : public ITask {
public:
	virtual ~QuitTask() {}
	virtual void execute() { printf("Processing quit Task\n"); }
};

class PhysTaskConv : public ITask {
public:
	virtual ~PhysTaskConv() {
		delete tsk;
	}
	virtual void execute() {
		tsk->execute();
	}

	IPhysicsTask* tsk;

};

class CEThread {

public:

	CEThread(ThreadManager& tm, uint32_t _id);
	virtual ~CEThread();

	void stopThread();

	void threadLoop();

	float lastTaskTime;
	uint64_t procCounts;

private:

	uint32_t id;
	std::atomic<bool> running;
	std::thread* thrd;
	Engine::System::HighResClock clock;

	ThreadManager& threadManager;

};

class ThreadManager : public IPhysThreadManager {

public:

	ThreadManager();
	virtual ~ThreadManager();

	virtual void queueTask(IPhysicsTask* task) {
		PhysTaskConv* ptc = new PhysTaskConv();
		ptc->tsk = task;
		threadTasks.enqueue(ptc);
	}

	virtual void queueTask(ITask* task) {
		threadTasks.enqueue(task);
	}

	ITask* nextTask();

	std::string getThreadInfo(int index);

private:

	SafeQueue<ITask*> threadTasks;

	CEThread* threads;

};


#endif