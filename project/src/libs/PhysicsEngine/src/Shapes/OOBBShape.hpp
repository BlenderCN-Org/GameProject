#ifndef OOBB_SHAPE_HPP
#define OOBB_SHAPE_HPP

/// Internal Inlcudes
#include "PhysicsEngine/IPhysShape.hpp"

/// External Includes
#pragma warning( push , 3 )
#include <glm/glm.hpp>
#pragma warning( pop )

class OOBBShape : public IPhysicsShape {

public:
	virtual ~OOBBShape() {};
	virtual PhysShapes getShapeType() const { return PhysShapes::OOBB; }

	glm::vec3 center;
	glm::vec3 rotation;
	glm::vec3 halfSize;
};

#endif