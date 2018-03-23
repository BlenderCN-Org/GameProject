#include "SAT.hpp"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace SAT {

	void getAxes(const OOBBShape& shape, Axis(&axes)[3]) {

		glm::vec3 up(0, 1, 0);
		glm::vec3 left(1, 0, 0);
		glm::vec3 forward(0, 0, 1);

		glm::quat qRot(glm::radians(shape.rotation));

		glm::mat4 rot(qRot);

		axes[0] = glm::vec3((glm::vec4(up, 1.0F) * rot));
		axes[1] = glm::vec3((glm::vec4(left, 1.0F) * rot));
		axes[2] = glm::vec3((glm::vec4(forward, 1.0F) * rot));

	}

	void crossAxesBBox(const Axis(&ax1)[3], const Axis(&ax2)[3], Axis(&out)[9]) {

		out[0] = glm::cross(ax1[0], ax2[0]);
		out[1] = glm::cross(ax1[0], ax2[1]);
		out[2] = glm::cross(ax1[0], ax2[2]);
		out[3] = glm::cross(ax1[1], ax2[0]);
		out[4] = glm::cross(ax1[1], ax2[1]);
		out[5] = glm::cross(ax1[1], ax2[2]);
		out[6] = glm::cross(ax1[2], ax2[0]);
		out[7] = glm::cross(ax1[2], ax2[1]);
		out[8] = glm::cross(ax1[2], ax2[2]);
	}

}