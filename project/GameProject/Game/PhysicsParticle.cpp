
/// Project Includes
#include "PhysicsParticle.hpp"

/// External Includes
#include <EngineCore/Core/System.hpp>
#include <PhysicsEngine/Shapes/SphereShape.hpp>

/// Std Includes

PhysicsParticle::PhysicsParticle(PhysicsEngine* pPhysEngine) {
	remove = false;
	physEngine = pPhysEngine;

	rb = physEngine->createRigidBody();
	rb->rsp = this;

	SphereShape* ss = new SphereShape();
	ss->center = glm::vec3(0);
	ss->radius = 0.1F;
	rb->shape = ss;

	rb->position = glm::vec3(0.0F, 1.0F, 0.0F);
	float randX = Engine::System::randomFloat(-2.0F, 2.0F);
	float randY = Engine::System::randomFloat(-2.0F, 2.0F);
	rb->velocity = glm::vec3(randX, 2.0F, randY);
}

PhysicsParticle::~PhysicsParticle() {
	delete rb->shape;
	physEngine->freeRigidBody(rb);
}

void PhysicsParticle::response() {
	remove = true;
}