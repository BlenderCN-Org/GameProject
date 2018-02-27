#ifndef PHYSICS_ENGINE_HPP
#define PHYSICS_ENGINE_HPP

/// Interna Includes
#include "RigidBody.hpp"
#include "StaticObject.hpp"

/// External Includes
#include <glm/glm.hpp>

/// Std Includes
#include <cstdint>
#include <vector>

class IPhysicsMemoryProfile {
public:

	virtual uint32_t getTotalMemory() = 0;
	virtual uint32_t getUsedMemory() = 0;
	virtual uint32_t getFreeMemory() = 0;

};

struct PhysEngineCreateInfo {
	bool threaded;

};

class PhysicsEngine {

public:

	PhysicsEngine();
	virtual ~PhysicsEngine();

	IPhysicsMemoryProfile* getPhysicsMemoryProfile();

	StaticObject* createStaticObject();
	RigidBody* createRigidBody();

	void freeStaticObject(StaticObject* objectId);
	void freeRigidBody(RigidBody* objectId);

	void update(float dt);

private:

	void pUpdate(float dt);
	void checkCollisions();
	void resolveCollisions();
	void checkConstraints();

	bool isColliding(IPhysicsShape* s1, RigidBody* rb);

	std::vector<StaticObject*> staticObjects;
	std::vector<RigidBody*> rigidBodys;

	glm::vec3 gravity;

};

#endif