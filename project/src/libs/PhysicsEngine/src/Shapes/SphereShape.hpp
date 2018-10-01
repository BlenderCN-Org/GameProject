#ifndef SPHERE_SHAPE_HPP
#define SPHERE_SHAPE_HPP

/// Internal Inlcudes
#include "PhysicsEngine/IPhysShape.hpp"

/// External Includes
#pragma warning( push , 3 )
#include <glm/glm.hpp>
#pragma warning( pop )

class SphereShape : public IPhysicsShape {

public:
	SphereShape() : center(), radius(0.0F) {}
	virtual ~SphereShape() {};
	virtual PhysShapes getShapeType() const { return PhysShapes::SPHERE; }

	glm::vec3 center;
	float radius;

};

#endif