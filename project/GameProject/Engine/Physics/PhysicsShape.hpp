#ifndef PHYSICS_SHAPE_HPP
#define PHYSICS_SHAPE_HPP

#include <glm/glm.hpp>

enum class PhysShape {
	RAY,
	PLANE,
	SPHERE,
	AABB,
	OOBB,
	MESH
};

class IPhysicsShape {

public:

	virtual ~IPhysicsShape() {};

	virtual bool collided(const IPhysicsShape& other) = 0;

	virtual PhysShape getShapeType() const = 0;

};

class PhysicsShape_Ray : public IPhysicsShape {

public:

	virtual bool collided(const IPhysicsShape& other);
	virtual PhysShape getShapeType() { return PhysShape::RAY; };

	glm::vec3 origin;
	glm::vec3 direction;

};


class PhysicsShape_Plane : public IPhysicsShape {

public:

	virtual bool collided(const IPhysicsShape& other);
	virtual PhysShape getShapeType() { return PhysShape::PLANE; };

	glm::vec3 origin;
	glm::vec3 normal;

};

class PhysicsShape_Sphere : public IPhysicsShape {

public:

	virtual bool collided(const IPhysicsShape& other);
	virtual PhysShape getShapeType() const { return PhysShape::SPHERE; };

	float radius;

	glm::vec3 center;

};

class PhysicsShape_AABB : public IPhysicsShape {

public:

	virtual bool collided(const IPhysicsShape& other);
	virtual PhysShape getShapeType() const { return PhysShape::AABB; };

	glm::vec3 origin;
	glm::vec3 halfsize;

};

class PhysicsShape_OOBB : public IPhysicsShape {

public:

	virtual bool collided(const IPhysicsShape& other);
	virtual PhysShape getShapeType() const { return PhysShape::OOBB; };

	glm::vec3 origin;
	glm::vec3 size;
	glm::vec3 upDirection;
};

class PhysicsShape_Mesh : public IPhysicsShape {

public:

	virtual bool collided(const IPhysicsShape& other);
	virtual PhysShape getShapeType() const { return PhysShape::MESH; };
};

#endif