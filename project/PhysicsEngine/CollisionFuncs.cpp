#include "CollisionFuncs.hpp"

#include <glm/glm.hpp>

bool PlaneVsSphere(const PlaneShape& p, const SphereShape& s) {

	bool collided = false;

	glm::vec3 po = p.normal * p.distance;
	glm::vec3 sp = s.center;// +rb->position;

	float d = abs(glm::dot(sp - po, p.normal));

	collided = (d < s.radius);

	return collided;
}

bool SphereVsSphere(const SphereShape& s1, const SphereShape& s2) {

	bool collided = false;

	float d = glm::distance(s1.center, s2.center);

	collided = (d < (s1.radius + s2.radius));

	return collided;
}

bool AABBVsAABB(const AABBShape& a1, const AABBShape& a2) {

	glm::vec3 o1 = a1.center;
	glm::vec3 o2 = a2.center;

	glm::vec3 h1 = a1.halfSize;
	glm::vec3 h2 = a2.halfSize;

	bool x = (abs(o1.x - o2.x) <= (h1.x + h2.x));
	bool y = (abs(o1.y - o2.y) <= (h1.y + h2.y));
	bool z = (abs(o1.z - o2.z) <= (h1.z + h2.z));

	return x && y && z;
}

bool AABBVsOOBB(const AABBShape& a, const OOBBShape& o) {

	return false;
}

bool OOBBVsOOBB(const OOBBShape& o1, const OOBBShape& o2) {
	return false;
}