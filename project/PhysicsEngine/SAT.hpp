#ifndef SAT_HPP
#define SAT_HPP

#include "Shapes/AABBShape.hpp"
#include "Shapes/OOBBShape.hpp"
#include <glm/glm.hpp>

namespace SAT {

	typedef glm::vec3 Axis;
	
	struct Dist
	{
		Dist()
		{
			min = glm::vec3(FLT_MAX);
			max = glm::vec3(-FLT_MAX);
		}
		glm::vec3 min;
		glm::vec3 max;

		void update(glm::vec3 v1)
		{
			min = glm::min(min, v1);
			max = glm::max(max, v1);
		}

	};

	void getAxes(const OOBBShape& shape, Axis(&axes)[3]);

	// takes axis from two boxes and combine to make 9 complementary axis.
	void crossAxesBBox(const Axis(&ax1)[3], const Axis(&ax2)[3], Axis(&out)[9]);

	glm::vec3 project(glm::vec3 point, const Axis& axis);

	Dist projectAABB(const AABBShape& shape, const Axis& a);

	Dist projectOOBB(const OOBBShape& shape, const Axis& a);

	bool isPointOnLine(glm::vec3 p, Dist line);

}
#endif