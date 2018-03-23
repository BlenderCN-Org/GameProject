#ifndef PLANE_SHAPE_HPP
#define PLANE_SHAPE_HPP

/// Internal Inlcudes
#include "IPhysShape.hpp"

/// External Includes
#include <glm/glm.hpp>

class PlaneShape : public IPhysicsShape {

public:
	virtual ~PlaneShape() {};

	virtual PhysShapes getShapeType() const { return PhysShapes::PLANE; }

	glm::vec3 normal;
	float distance;

};

#endif