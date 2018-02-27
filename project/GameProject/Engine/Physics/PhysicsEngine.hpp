#ifndef PHYSICS_ENGINE_OLD_HPP
#define PHYSICS_ENGINE_OLD_HPP

/// Internal Includes
#include "PhysicsShape.hpp"

/// External Includes
#include <glm/glm.hpp>

/// Std Includes
#include <vector>


namespace Engine {
	namespace Physics {

		struct PhysicsObject {

			glm::vec3 position;
			glm::vec3 velocity;

		};

		class PhysicsEngine {
		public:

			PhysicsEngine();
			virtual ~PhysicsEngine();

			void freezeFlag(bool freeze);

			void update(float dt);

			PhysicsShape_AABB aabb;
			PhysicsShape_AABB aabb2;


		private:

			void applyForces(uint32_t start, uint32_t end, float dt);
			void detectCollisions();
			void collisionReaction();

			bool frozen;

			glm::vec3 gravity;

			std::vector<PhysicsObject*> physObjects;
			std::vector<PhysicsObject*> staticObjects;

		};
	}
}

#endif