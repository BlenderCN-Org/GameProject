#ifndef PHYSICS_ENGINE_HPP
#define PHYSICS_ENGINE_HPP

/// Internal Includes

/// External Includes
#include <glm/glm.hpp>

/// Std Includes



namespace Engine {
	namespace Physics {

		class PhysicsEngine {
		public:

			PhysicsEngine();
			virtual ~PhysicsEngine();

			void update(float dt);

		private:

			glm::vec3 gravity;

		};
	}
}

#endif