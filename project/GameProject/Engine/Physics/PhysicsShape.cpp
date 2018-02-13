#include "PhysicsShape.hpp"

#include <glm/glm.hpp>

/*
Ray
*/

bool rayVSRay(const PhysicsShape_Ray* const shape, const PhysicsShape_Ray* const shape2) {

	bool c = false;

	if (shape == shape2) {
		c = true;
	} else {

		glm::vec3 o1 = shape->origin;
		glm::vec3 o2 = shape2->origin;

		glm::vec3 d1 = shape->direction;
		glm::vec3 d2 = shape2->direction;

		glm::vec3 cr = glm::cross(d1, d2);
		float denom = glm::pow(glm::length(cr), 2.0F);

		if (denom != 0) {

			glm::mat3 m1 = glm::mat3(o2 - o1, d2, cr);
			glm::mat3 m2 = glm::mat3(o2 - o1, d1, cr);

			float t1 = glm::determinant(m1) / denom;
			float t2 = glm::determinant(m2) / denom;

			glm::vec3 l1 = o1 + d1 * t1;
			glm::vec3 l2 = o2 + d2 * t2;

			if (abs(l1.x - l2.x) < 0.001F &&
				abs(l1.y - l2.y) < 0.001F &&
				abs(l1.z - l2.z) < 0.001F) {
				c = true;
			}
		}

	}

	return c;
}

bool rayVSPlane(const PhysicsShape_Ray* const shape, const PhysicsShape_Plane* const shape2) {
	return false;
}

bool rayVSSphere(const PhysicsShape_Ray* const shape, const PhysicsShape_Sphere* const shape2) {

	float r = shape2->radius;

	glm::vec3 m = shape->origin - shape2->center;
	float b = glm::dot(m, shape->direction);
	float c = glm::dot(m, m) - (r * r);

	if (c > 0.0f && b > 0.0f) {
		return false;
	}

	float discr = (b*b) - c;
	if (discr < 0.0f) {
		return false;
	}

	return true;
}

bool rayVSAABB(const PhysicsShape_Ray* const shape, const PhysicsShape_AABB* const shape2) {
	return false;
}

bool rayVSOOBB(const PhysicsShape_Ray* const shape, const PhysicsShape_OOBB* const shape2) {
	return false;
}

bool rayVSMesh(const PhysicsShape_Ray* const shape, const PhysicsShape_Mesh* const shape2) {
	return false;
}


/*
PLANE
*/
bool planeVSPlane(const PhysicsShape_Plane* const shape, const PhysicsShape_Plane* const shape2) {
	return false;
}

bool planeVSSphere(const PhysicsShape_Plane* const shape, const PhysicsShape_Sphere* const shape2) {
	return false;
}

bool planeVSAABB(const PhysicsShape_Plane* const shape, const PhysicsShape_AABB* const shape2) {
	return false;
}

bool planeVSOOBB(const PhysicsShape_Plane* const shape, const PhysicsShape_OOBB* const shape2) {
	return false;
}

bool planeVSMesh(const PhysicsShape_Plane* const shape, const PhysicsShape_Mesh* const shape2) {
	return false;
}

/*
SPHERE
*/

bool sphereVSSphere(const PhysicsShape_Sphere* const shape, const PhysicsShape_Sphere* const shape2) {


	float d = glm::distance(shape->center, shape2->center);
	float d2 = shape->radius + shape2->radius;

	bool c = false;

	if (d < d2) {
		c = true;
	}

	return c;
}

bool sphereVSAABB(const PhysicsShape_Sphere* const shape, const PhysicsShape_AABB* const shape2) {
	return false;
}

bool sphereVSOOBB(const PhysicsShape_Sphere* const shape, const PhysicsShape_OOBB* const shape2) {
	return false;
}

bool sphereVSMesh(const PhysicsShape_Sphere* const shape, const PhysicsShape_Mesh* const shape2) {
	return false;
}

/*
AABB
*/

bool AABBVSAABB(const PhysicsShape_AABB* const shape, const PhysicsShape_AABB* const shape2) {

	//glm::vec3 min1 = shape->origin - shape->halfsize;
	//glm::vec3 max1 = shape->origin + shape->halfsize;
	//
	//glm::vec3 min2 = shape2->origin - shape2->halfsize;
	//glm::vec3 max2 = shape2->origin + shape2->halfsize;
	//
	//max1 += FLT_EPSILON;
	//min1 -= FLT_EPSILON;
	//
	//max2 += FLT_EPSILON;
	//min2 -= FLT_EPSILON;
	//
	//if (max1.x > min2.x && min1.x < max2.x)//x
	//	if (max1.y > min2.y && min1.y < max2.y)//y
	//		if (max1.z > min2.z && min1.z < max2.z)//z
	//			return true;
	//
	//return false;


	glm::vec3 o1 = shape->origin;
	glm::vec3 o2 = shape2->origin;
	
	glm::vec3 h1 = shape->halfsize;
	glm::vec3 h2 = shape2->halfsize;
	
	bool x = (abs(o1.x - o2.x) <= (h1.x + h2.x));
	bool y = (abs(o1.y - o2.y) <= (h1.y + h2.y));
	bool z = (abs(o1.z - o2.z) <= (h1.z + h2.z));
	
	
	return x && y && z;
}

bool AABBVSOOBB(const PhysicsShape_AABB* const shape, const PhysicsShape_OOBB* const shape2) {
	return false;
}

bool AABBVSMesh(const PhysicsShape_AABB* const shape, const PhysicsShape_Mesh* const shape2) {
	return false;
}

/*
OOBB
*/

bool OOBBVSOOBB(const PhysicsShape_OOBB* const shape, const PhysicsShape_OOBB* const shape2) {
	return false;
}

bool OOBBVSMesh(const PhysicsShape_OOBB* const shape, const PhysicsShape_Mesh* const shape2) {
	return false;
}

/*
MESH
*/

bool meshVSMesh(const PhysicsShape_Mesh* const shape, const PhysicsShape_Mesh* const shape2) {
	return false;
}



bool PhysicsShape_Ray::collided(const IPhysicsShape & other) {

	bool collided = false;

	PhysShape otherShape = other.getShapeType();

	switch (otherShape) {
	case PhysShape::RAY:
		collided = rayVSRay(this, (PhysicsShape_Ray*)&other);
		break;
	case PhysShape::PLANE:
		collided = rayVSPlane(this, (PhysicsShape_Plane*)&other);
		break;
	case PhysShape::SPHERE:
		collided = rayVSSphere(this, (PhysicsShape_Sphere*)&other);
		break;
	case PhysShape::AABB:
		collided = rayVSAABB(this, (PhysicsShape_AABB*)&other);
		break;
	case PhysShape::OOBB:
		collided = rayVSOOBB(this, (PhysicsShape_OOBB*)&other);
		break;
	case PhysShape::MESH:
		collided = rayVSMesh(this, (PhysicsShape_Mesh*)&other);
		break;
	default:
		break;
	}

	return collided;

}

bool PhysicsShape_Plane::collided(const IPhysicsShape & other) {

	bool collided = false;

	PhysShape otherShape = other.getShapeType();

	switch (otherShape) {
	case PhysShape::RAY:
		collided = rayVSPlane((PhysicsShape_Ray*)&other, this);
		break;
	case PhysShape::PLANE:
		collided = planeVSPlane(this, (PhysicsShape_Plane*)&other);
		break;
	case PhysShape::SPHERE:
		collided = planeVSSphere(this, (PhysicsShape_Sphere*)&other);
		break;
	case PhysShape::AABB:
		collided = planeVSAABB(this, (PhysicsShape_AABB*)&other);
		break;
	case PhysShape::OOBB:
		collided = planeVSOOBB(this, (PhysicsShape_OOBB*)&other);
		break;
	case PhysShape::MESH:
		collided = planeVSMesh(this, (PhysicsShape_Mesh*)&other);
		break;
	default:
		break;
	}

	return collided;
}

bool PhysicsShape_Sphere::collided(const IPhysicsShape & other) {
	bool collided = false;

	PhysShape otherShape = other.getShapeType();

	switch (otherShape) {
	case PhysShape::RAY:
		collided = rayVSSphere((PhysicsShape_Ray*)&other, this);
		break;
	case PhysShape::PLANE:
		collided = planeVSSphere((PhysicsShape_Plane*)&other, this);
		break;
	case PhysShape::SPHERE:
		collided = sphereVSSphere(this, (PhysicsShape_Sphere*)&other);
		break;
	case PhysShape::AABB:
		collided = sphereVSAABB(this, (PhysicsShape_AABB*)&other);
		break;
	case PhysShape::OOBB:
		collided = sphereVSOOBB(this, (PhysicsShape_OOBB*)&other);
		break;
	case PhysShape::MESH:
		collided = sphereVSMesh(this, (PhysicsShape_Mesh*)&other);
		break;
	default:
		break;
	}

	return collided;
}

bool PhysicsShape_AABB::collided(const IPhysicsShape & other) {
	bool collided = false;

	PhysShape otherShape = other.getShapeType();

	switch (otherShape) {
	case PhysShape::RAY:
		collided = rayVSAABB((PhysicsShape_Ray*)&other, this);
		break;
	case PhysShape::PLANE:
		collided = planeVSAABB((PhysicsShape_Plane*)&other, this);
		break;
	case PhysShape::SPHERE:
		collided = sphereVSAABB((PhysicsShape_Sphere*)&other, this);
		break;
	case PhysShape::AABB:
		collided = AABBVSAABB(this, (PhysicsShape_AABB*)&other);
		break;
	case PhysShape::OOBB:
		collided = AABBVSOOBB(this, (PhysicsShape_OOBB*)&other);
		break;
	case PhysShape::MESH:
		collided = AABBVSMesh(this, (PhysicsShape_Mesh*)&other);
		break;
	default:
		break;
	}

	return collided;

}

bool PhysicsShape_OOBB::collided(const IPhysicsShape & other) {
	bool collided = false;

	PhysShape otherShape = other.getShapeType();

	switch (otherShape) {
	case PhysShape::RAY:
		collided = rayVSOOBB((PhysicsShape_Ray*)&other, this);
		break;
	case PhysShape::PLANE:
		collided = planeVSOOBB((PhysicsShape_Plane*)&other, this);
		break;
	case PhysShape::SPHERE:
		collided = sphereVSOOBB((PhysicsShape_Sphere*)&other, this);
		break;
	case PhysShape::AABB:
		collided = AABBVSOOBB((PhysicsShape_AABB*)&other, this);
		break;
	case PhysShape::OOBB:
		collided = OOBBVSOOBB(this, (PhysicsShape_OOBB*)&other);
		break;
	case PhysShape::MESH:
		collided = OOBBVSMesh(this, (PhysicsShape_Mesh*)&other);
		break;
	default:
		break;
	}

	return collided;

}

bool PhysicsShape_Mesh::collided(const IPhysicsShape & other) {
	bool collided = false;

	PhysShape otherShape = other.getShapeType();

	switch (otherShape) {
	case PhysShape::RAY:
		collided = rayVSMesh((PhysicsShape_Ray*)&other, this);
		break;
	case PhysShape::PLANE:
		collided = planeVSMesh((PhysicsShape_Plane*)&other, this);
		break;
	case PhysShape::SPHERE:
		collided = sphereVSMesh((PhysicsShape_Sphere*)&other, this);
		break;
	case PhysShape::AABB:
		collided = AABBVSMesh((PhysicsShape_AABB*)&other, this);
		break;
	case PhysShape::OOBB:
		collided = OOBBVSMesh((PhysicsShape_OOBB*)&other, this);
		break;
	case PhysShape::MESH:
		collided = meshVSMesh(this, (PhysicsShape_Mesh*)&other);
		break;
	default:
		break;
	}

	return collided;
}