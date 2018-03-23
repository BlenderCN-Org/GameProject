#ifndef AABB_SHAPE_HPP
#define AABB_SHAPE_HPP

/// Internal Inlcudes
#include "IPhysShape.hpp"

/// External Includes
#include <glm/glm.hpp>

class AABBShape : public IPhysicsShape {

public:
	virtual ~AABBShape() {};
	virtual PhysShapes getShapeType() const { return PhysShapes::AABB; }

	glm::vec3 center;
	glm::vec3 halfSize;
};

#endif