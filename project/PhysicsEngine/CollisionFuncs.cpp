#include "CollisionFuncs.hpp"
#include "SAT.hpp"

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

	SAT::Axis obbAxes[3];
	SAT::Axis aabbAxes[3];

	SAT::Axis otherAxes[9];

	aabbAxes[0] = glm::vec3(0, 1, 0);
	aabbAxes[1] = glm::vec3(1, 0, 0);
	aabbAxes[2] = glm::vec3(0, 0, 1);

	SAT::getAxes(o, obbAxes);

	SAT::crossAxesBBox(obbAxes, aabbAxes, otherAxes);

	typedef bool(*PointCheck)(glm::vec3, SAT::Dist);
	PointCheck pCheck = SAT::isPointOnLine;

	SAT::Dist d1[15];
	SAT::Dist d2[15];

	d1[0] = SAT::projectAABB(a, obbAxes[0]);
	d2[0] = SAT::projectOOBB(o, obbAxes[0]);

	d1[1] = SAT::projectAABB(a, obbAxes[1]);
	d2[1] = SAT::projectOOBB(o, obbAxes[1]);

	d1[2] = SAT::projectAABB(a, obbAxes[2]);
	d2[2] = SAT::projectOOBB(o, obbAxes[2]);

	d1[3] = SAT::projectAABB(a, aabbAxes[0]);
	d2[3] = SAT::projectOOBB(o, aabbAxes[0]);

	d1[4] = SAT::projectAABB(a, aabbAxes[1]);
	d2[4] = SAT::projectOOBB(o, aabbAxes[1]);

	d1[5] = SAT::projectAABB(a, aabbAxes[2]);
	d2[5] = SAT::projectOOBB(o, aabbAxes[2]);

	d1[6] = SAT::projectAABB(a, otherAxes[0]);
	d2[6] = SAT::projectOOBB(o, otherAxes[0]);

	d1[7] = SAT::projectAABB(a, otherAxes[1]);
	d2[7] = SAT::projectOOBB(o, otherAxes[1]);

	d1[8] = SAT::projectAABB(a, otherAxes[2]);
	d2[8] = SAT::projectOOBB(o, otherAxes[2]);

	d1[9] = SAT::projectAABB(a, otherAxes[3]);
	d2[9] = SAT::projectOOBB(o, otherAxes[3]);

	d1[10] = SAT::projectAABB(a, otherAxes[4]);
	d2[10] = SAT::projectOOBB(o, otherAxes[4]);

	d1[11] = SAT::projectAABB(a, otherAxes[5]);
	d2[11] = SAT::projectOOBB(o, otherAxes[5]);

	d1[12] = SAT::projectAABB(a, otherAxes[6]);
	d2[12] = SAT::projectOOBB(o, otherAxes[6]);

	d1[13] = SAT::projectAABB(a, otherAxes[7]);
	d2[13] = SAT::projectOOBB(o, otherAxes[7]);

	d1[14] = SAT::projectAABB(a, otherAxes[8]);
	d2[14] = SAT::projectOOBB(o, otherAxes[8]);
	
	bool b[15];

	for ( int i = 0; i < 15; i++ )
	{
		b[i] = pCheck(d1[i].max, d2[i]) || pCheck(d1[i].min, d2[i]) || pCheck(d2[i].max, d1[i]) || pCheck(d2[i].min, d1[i]);
	}

	return  b[0] &&
			b[1] && 
			b[2] &&
			b[3] &&
			b[4] &&
			b[5] &&
			b[6] &&
			b[7] &&
			b[8] &&
			b[9] &&
			b[10] &&
			b[11] &&
			b[12] &&
			b[13] &&
			b[14];
}

bool OOBBVsOOBB(const OOBBShape& o1, const OOBBShape& o2) {
	return false;
}