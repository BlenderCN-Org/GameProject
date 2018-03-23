#ifndef OOBB_SHAPE_HPP
#define OOBB_SHAPE_HPP

/// Internal Inlcudes
#include "IPhysShape.hpp"

/// External Includes
#include <glm/glm.hpp>

class OOBBShape : public IPhysicsShape {

public:
	virtual ~OOBBShape() {};
	virtual PhysShapes getShapeType() const { return PhysShapes::OOBB; }

	glm::vec3 center;
	glm::vec3 rotation;
	glm::vec3 halfSize;
};

#endif