#ifndef AABB_SHAPE_HPP
#define AABB_SHAPE_HPP

/// Internal Inlcudes
#include "PhysicsEngine/IPhysShape.hpp"

/// External Includes
#pragma warning( push , 3 )
#include <glm/glm.hpp>
#pragma warning( pop )

class AABBShape : public IPhysicsShape {

public:
	virtual ~AABBShape() {};
	virtual PhysShapes getShapeType() const { return PhysShapes::AABB; }

	glm::vec3 center;
	glm::vec3 halfSize;
};

#endif