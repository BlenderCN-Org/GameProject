/// Internal Includes
#include "PhysicsEngine.hpp"

namespace Engine {
	namespace Physics {

		// Note that Y Is up/down vector
		PhysicsEngine::PhysicsEngine()
			: gravity(0.0F, 9.82F, 0.0F)
			, frozen(true) {

			PhysicsObject* pobj = new PhysicsObject();
			pobj->position = glm::vec3(0.0F, 10.0F, 0.0F);
			pobj->velocity = glm::vec3(0.0F);

			physObjects.push_back(pobj);

			aabb.origin = glm::vec3(0.0F, 0.0F, 0.0F);
			aabb.halfsize = glm::vec3(0.5F, 0.5F, 0.5F);

			aabb2.origin = glm::vec3(0.0F, 2.0F, 0.0F);
			aabb2.halfsize = glm::vec3(0.25F, 0.25F, 0.25F);

		}

		PhysicsEngine::~PhysicsEngine() {

			delete physObjects[0];

		}

		void PhysicsEngine::freezeFlag(bool freeze) {
			frozen = freeze;
		}

		void PhysicsEngine::update(float dt) {

			if (!frozen) {

				//if (physObjects[0]->position.y > 0.0001F) {
				//	PhysicsObject* p = physObjects[0];
				//	//printf("Physobj Pos: (%f, %f, %f)\n", p->position.x, p->position.y, p->position.z);
				//	//printf("Physobj vel: (%f, %f, %f)\n", p->velocity.x, p->velocity.y, p->velocity.z);
				//}
				//
				//applyForces(0, physObjects.size(), dt);
				//detectCollisions();
				//collisionReaction();

				if (aabb.collided(aabb2)) {
					printf("AABB Collision\n");
					printf("   (%f, %f, %f)\n", aabb2.origin.x, aabb2.origin.y, aabb2.origin.z);
				}

				//aabb2.origin.x -= 0.01F;
				aabb2.origin.y -= 0.01F;

			}
		}

		/****************************************
		*			PRIVATE FUNCTIONS			*
		*****************************************/

		void PhysicsEngine::applyForces(uint32_t start, uint32_t end, float dt) {

			for (size_t i = start; i < end; i++) {

				physObjects[i]->velocity -= gravity * dt;
				physObjects[i]->position += physObjects[i]->velocity * dt;
			}

		}

		void PhysicsEngine::detectCollisions() {

			for (size_t i = 0; i < physObjects.size(); i++) {

				if (physObjects[i]->position.y < 0.0F) {
					physObjects[i]->position.y = 0.0F;
					physObjects[i]->velocity *= -0.8F;
				}
			}
		}

		void PhysicsEngine::collisionReaction() {

		}

	}
}