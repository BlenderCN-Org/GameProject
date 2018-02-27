#ifndef SPHERE_SHAPE_HPP
#define SPHERE_SHAPE_HPP

/// Internal Inlcudes
#include "IPhysShape.hpp"

/// External Includes
#include <glm/glm.hpp>

class SphereShape : public IPhysicsShape {

public:
	virtual ~SphereShape() {};

	glm::vec3 center;
	float radius;

};

#endif