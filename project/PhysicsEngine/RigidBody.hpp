#ifndef RIGID_BODY_HPP
#define RIGID_BODY_HPP

/// Internal Includes
#include "Shapes/IPhysShape.hpp"

/// External Includes
#include <glm/glm.hpp>

/// Std Includes

class RigidBody {
public:

	glm::vec3 velocity;
	glm::vec3 position;

	IPhysicsShape * shape;
};

#endif