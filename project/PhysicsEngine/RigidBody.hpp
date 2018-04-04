#ifndef RIGID_BODY_HPP
#define RIGID_BODY_HPP

/// Internal Includes
#include "Shapes/IPhysShape.hpp"

/// External Includes
#include <glm/glm.hpp>

/// Std Includes

class ICollisionResponse {
public:
	virtual ~ICollisionResponse() {};
	virtual void response() = 0;
};

class RigidBody {
public:

	glm::vec3 velocity;
	glm::vec3 position;
	glm::vec3 oldPos;

	IPhysicsShape * shape;
	ICollisionResponse* rsp;
};

#endif