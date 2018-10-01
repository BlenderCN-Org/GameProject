#ifndef I_PHYS_SHAPE_HPP
#define I_PHYS_SHAPE_HPP

enum class PhysShapes {
	RAY = 0,
	PLANE = 1,
	SPHERE = 2,
	AABB = 3,
	OOBB = 4,
	TRIANGLE = 5,
};

class IPhysicsShape {

public:
	virtual ~IPhysicsShape() {};
	virtual PhysShapes getShapeType() const = 0;

};

#endif