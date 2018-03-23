#ifndef SAT_HPP
#define SAT_HPP

#include "Shapes/OOBBShape.hpp"
#include <glm/glm.hpp>

namespace SAT {

	typedef glm::vec3 Axis;

	void getAxes(const OOBBShape& shape, Axis(&axes)[3]);

	// takes axis from two boxes and combine to make 9 complementary axis.
	void crossAxesBBox(const Axis(&ax1)[3], const Axis(&ax2)[3], Axis(&out)[9]);

}
#endif