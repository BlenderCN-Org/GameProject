#ifndef PLANE_SHAPE_HPP
#define PLANE_SHAPE_HPP

/// Internal Inlcudes
#include "PhysicsEngine/IPhysShape.hpp"

/// External Includes
#pragma warning( push , 3 )
#include <glm/glm.hpp>
#pragma warning( pop )

class PlaneShape : public IPhysicsShape {

public:
	PlaneShape() : normal(), distance(0.0F) {}
	virtual ~PlaneShape() {};

	virtual PhysShapes getShapeType() const { return PhysShapes::PLANE; }

	glm::vec3 normal;
	float distance;

};

#endif