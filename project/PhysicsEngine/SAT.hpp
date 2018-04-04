#ifndef SAT_HPP
#define SAT_HPP

#include "Shapes/AABBShape.hpp"
#include "Shapes/OOBBShape.hpp"
#include <glm/glm.hpp>

namespace SAT {

	typedef glm::vec3 Axis;
	
	struct Projection {

		Projection() {
			min = FLT_MAX;
			max = -FLT_MIN;
		}

		void update(float v) {
			min = glm::min(min, v);
			max = glm::max(max, v);
		}

		float min;
		float max;

	};
	
	void getAxes(const OOBBShape& shape, Axis(&axes)[3]);

	// takes axis from two boxes and combine to make 9 complementary axis.
	void crossAxesBBox(const Axis(&ax1)[3], const Axis(&ax2)[3], Axis(&out)[9]);

	glm::vec3 project(glm::vec3 point, const Axis& axis);

	float fltProj(glm::vec3 point, const Axis& axis);

	Projection projectAABB(const AABBShape& shape, const Axis& a);

	Projection projectOOBB(const OOBBShape& shape, const Axis& a);

	bool projectionOverlap(const Projection &p1, const Projection &p2);

}
#endif