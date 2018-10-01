#ifndef PHYSICS_ENGINE_HPP
#define PHYSICS_ENGINE_HPP

/// Interna Includes
#include "RigidBody.hpp"
#include "StaticObject.hpp"

/// External Includes
#pragma warning( push , 3 )
#include <glm/glm.hpp>
#pragma warning( pop )

/// Std Includes
#include <cstdint>
#include <vector>

class IPhysicsMemoryProfile {
public:

	virtual uint32_t getTotalMemory() = 0;
	virtual uint32_t getUsedMemory() = 0;
	virtual uint32_t getFreeMemory() = 0;

};

class IPhysicsTask {
public:
	virtual ~IPhysicsTask() {};
	virtual void execute() = 0;
};

class IPhysThreadManager {
public:
	virtual ~IPhysThreadManager() {};
	virtual void queueTask(IPhysicsTask* task) = 0;
};

struct PhysEngineCreateInfo {
	bool threaded;
	uint32_t maxTasks;
	IPhysThreadManager* taskMgr;
};

class PhysicsEngine {

	friend class PhysicsUpdateTask;

public:

	PhysicsEngine(PhysEngineCreateInfo &createInfo);
	virtual ~PhysicsEngine();

	IPhysicsMemoryProfile* getPhysicsMemoryProfile();

	StaticObject* createStaticObject();
	RigidBody* createRigidBody();

	void freeStaticObject(StaticObject* objectId);
	void freeRigidBody(RigidBody* objectId);

	void update(const float dt);

	uint32_t getNumRigidBodies() const;
	const RigidBody* const* getRigidObjects() const;

private:

	void pUpdate(const float dt);
	void checkCollisions();
	void resolveCollisions();
	void checkConstraints();

	bool isColliding(IPhysicsShape* s1, RigidBody* rb);

	inline void rigidBodyUpdate(RigidBody* rb, float dt);

	std::vector<StaticObject*> staticObjects;
	std::vector<RigidBody*> rigidBodys;

	glm::vec3 gravity;

	PhysEngineCreateInfo engineInfo;

};

#endif