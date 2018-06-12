#include "PhysicsEngine.hpp"

#include "Shapes/PlaneShape.hpp"
#include "Shapes/SphereShape.hpp"

#include "CollisionFuncs.hpp"

#include <atomic>
#include <time.h>

class PhysicsUpdateTask : public IPhysicsTask {

public:
	PhysicsUpdateTask(const float _dt) : dt(_dt) {}
	virtual ~PhysicsUpdateTask() {};

	virtual void execute() {

		const uint32_t end = startOffset + count;

		for (uint32_t i = startOffset; i < end; i++) {

			pEngine->rigidBodyUpdate(rbs[i], dt);
		}
		atInt->fetch_sub(1);
	}

	const float dt;

	RigidBody** rbs;
	uint32_t startOffset;
	uint32_t count;
	PhysicsEngine* pEngine;

	std::atomic<int>* atInt;

};

PhysicsEngine::PhysicsEngine(PhysEngineCreateInfo &createInfo)
	: gravity(0.0F, 9.82F, 0.0F)
	, engineInfo(createInfo) {
	int b = 0;
}

PhysicsEngine::~PhysicsEngine() {

}

IPhysicsMemoryProfile* PhysicsEngine::getPhysicsMemoryProfile() {
	return nullptr;
}

StaticObject* PhysicsEngine::createStaticObject() {

	StaticObject* obj = new StaticObject();
	obj->shape = nullptr;

	staticObjects.push_back(obj);

	return obj;
}

RigidBody* PhysicsEngine::createRigidBody() {

	RigidBody* obj = new RigidBody();

	obj->rsp = nullptr;
	obj->shape = nullptr;

	rigidBodys.push_back(obj);

	return obj;
}

void PhysicsEngine::freeStaticObject(StaticObject* objectId) {
	delete objectId;
}

void PhysicsEngine::freeRigidBody(RigidBody* objectId) {

	std::vector<RigidBody*>::iterator it = std::find(rigidBodys.begin(), rigidBodys.end(), objectId);
	rigidBodys.erase(it);

	delete objectId;
}

void PhysicsEngine::update(const float dt) {

	pUpdate(dt);
	checkCollisions();
	resolveCollisions();
	checkConstraints();
}

uint32_t PhysicsEngine::getNumRigidBodies() const {
	return (uint32_t)rigidBodys.size();
}

const RigidBody* const* PhysicsEngine::getRigidObjects() const {

	return rigidBodys.data();
}

/**********************
*  PRIVATE FUNCTIONS  *
***********************/

void PhysicsEngine::pUpdate(const float dt) {

	if (engineInfo.threaded) {

		RigidBody** rbs = rigidBodys.data();

		uint32_t count = (uint32_t)rigidBodys.size();

		if (engineInfo.maxTasks) {

			uint32_t eCount = count / engineInfo.maxTasks;

			uint32_t offset = 0;

			std::atomic<int> atInt = engineInfo.maxTasks;

			for (uint32_t i = 0; i < engineInfo.maxTasks; i++) {

				PhysicsUpdateTask* put = new PhysicsUpdateTask(dt);

				if (i == engineInfo.maxTasks - 1) {
					eCount += (count - (offset + eCount));
				}

				put->rbs = rbs;
				put->pEngine = this;
				put->startOffset = offset;
				put->count = eCount;
				put->atInt = &atInt;

				engineInfo.taskMgr->queueTask(put);

				offset += eCount;
			}

			while (atInt != 0);
		}

	} else {
		for (size_t i = 0; i < rigidBodys.size(); i++) {
			RigidBody* rb = rigidBodys[i];
			rigidBodyUpdate(rb, dt);
			//printf("Pos %f\n", rb->position.y);
		}
	}
}

void PhysicsEngine::checkCollisions() {

	for (size_t s = 0; s < staticObjects.size(); s++) {

		for (size_t r = 0; r < rigidBodys.size(); r++) {

			if (isColliding(staticObjects[s]->shape, rigidBodys[r])) {
				//rigidBodys[r]->velocity *= -1;
				rigidBodys[r]->velocity = glm::reflect(rigidBodys[r]->velocity, glm::vec3(0, 1, 0));
				rigidBodys[r]->rsp->response();
				//printf("Collision\n");
			}
		}
	}

}

void PhysicsEngine::resolveCollisions() {

}

void PhysicsEngine::checkConstraints() {

}

bool PhysicsEngine::isColliding(IPhysicsShape* s1, RigidBody* rb) {
	bool collided = false;

	//PhysShapes shape1 = s1->getShapeType();
	//PhysShapes shape2 = rb->shape->getShapeType();



	PlaneShape* ps = (PlaneShape*)s1;
	SphereShape* ss = (SphereShape*)rb->shape;

	SphereShape ts = *ss;
	ts.center += rb->position;

	collided = PlaneVsSphere(*ps, ts);

	//glm::vec3 po = ps->normal * ps->distance;
	//glm::vec3 sp = ss->center + rb->position;
	//
	//float d = abs(glm::dot(sp - po, ps->normal));
	//
	//if (d < ss->radius) {
	//	collided = true;
	//}

	// might have passed through object due to high speed
	if (!collided) {

		//rb-> oldPos

	}

	return collided;
}

inline void PhysicsEngine::rigidBodyUpdate(RigidBody* rb, float dt) {
	rb->velocity -= gravity * dt;
	rb->oldPos = rb->position;
	rb->position += rb->velocity * dt;
}