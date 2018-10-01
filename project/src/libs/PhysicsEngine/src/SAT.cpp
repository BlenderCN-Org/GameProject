#include "SAT.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#pragma warning( push , 3 )
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/projection.hpp>
#pragma warning( pop )

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

	glm::vec3 project(glm::vec3 point, const Axis& axis) {

		glm::vec3 result = glm::proj(point, axis);
		return result;
	}

	float fltProj(glm::vec3 point, const Axis& axis) {
		return (point.x * axis.x) + (point.y * axis.y) + (point.z * axis.z);
	}
	
	Projection projectAABB(const AABBShape& shape, const Axis& a) {

		Projection p;

		glm::vec3 hs = shape.halfSize;

		glm::vec3 p1 = shape.center + glm::vec3(+hs.x, +hs.y, +hs.z);
		glm::vec3 p2 = shape.center + glm::vec3(+hs.x, +hs.y, -hs.z);
		glm::vec3 p3 = shape.center + glm::vec3(+hs.x, -hs.y, +hs.z);
		glm::vec3 p4 = shape.center + glm::vec3(+hs.x, -hs.y, -hs.z);

		glm::vec3 p5 = shape.center + glm::vec3(-hs.x, +hs.y, +hs.z);
		glm::vec3 p6 = shape.center + glm::vec3(-hs.x, +hs.y, -hs.z);
		glm::vec3 p7 = shape.center + glm::vec3(-hs.x, -hs.y, +hs.z);
		glm::vec3 p8 = shape.center + glm::vec3(-hs.x, -hs.y, -hs.z);

		float fp1 = fltProj(p1, a);
		float fp2 = fltProj(p2, a);
		float fp3 = fltProj(p3, a);
		float fp4 = fltProj(p4, a);
		float fp5 = fltProj(p5, a);
		float fp6 = fltProj(p6, a);
		float fp7 = fltProj(p7, a);
		float fp8 = fltProj(p8, a);

		p.update(fp1);
		p.update(fp2);
		p.update(fp3);
		p.update(fp4);
		p.update(fp5);
		p.update(fp6);
		p.update(fp7);
		p.update(fp8);

		return p;
	}

	Projection projectOOBB(const OOBBShape& shape, const Axis& a) {

		Projection p;

		glm::quat qRot(glm::radians(shape.rotation));

		glm::mat4 rot(qRot);

		glm::vec3 hs = shape.halfSize;

		glm::vec3 p1 = shape.center + glm::vec3(+hs.x, +hs.y, +hs.z);
		glm::vec3 p2 = shape.center + glm::vec3(+hs.x, +hs.y, -hs.z);
		glm::vec3 p3 = shape.center + glm::vec3(+hs.x, -hs.y, +hs.z);
		glm::vec3 p4 = shape.center + glm::vec3(+hs.x, -hs.y, -hs.z);

		glm::vec3 p5 = shape.center + glm::vec3(-hs.x, +hs.y, +hs.z);
		glm::vec3 p6 = shape.center + glm::vec3(-hs.x, +hs.y, -hs.z);
		glm::vec3 p7 = shape.center + glm::vec3(-hs.x, -hs.y, +hs.z);
		glm::vec3 p8 = shape.center + glm::vec3(-hs.x, -hs.y, -hs.z);

		p1 = glm::vec3(glm::vec4(p1, 1.0F) * rot);
		p2 = glm::vec3(glm::vec4(p2, 1.0F) * rot);
		p3 = glm::vec3(glm::vec4(p3, 1.0F) * rot);
		p4 = glm::vec3(glm::vec4(p4, 1.0F) * rot);
		p5 = glm::vec3(glm::vec4(p5, 1.0F) * rot);
		p6 = glm::vec3(glm::vec4(p6, 1.0F) * rot);
		p7 = glm::vec3(glm::vec4(p7, 1.0F) * rot);
		p8 = glm::vec3(glm::vec4(p8, 1.0F) * rot);

		float fp1 = fltProj(p1, a);
		float fp2 = fltProj(p2, a);
		float fp3 = fltProj(p3, a);
		float fp4 = fltProj(p4, a);
		float fp5 = fltProj(p5, a);
		float fp6 = fltProj(p6, a);
		float fp7 = fltProj(p7, a);
		float fp8 = fltProj(p8, a);

		p.update(fp1);
		p.update(fp2);
		p.update(fp3);
		p.update(fp4);
		p.update(fp5);
		p.update(fp6);
		p.update(fp7);
		p.update(fp8);

		return p;

	}

	bool projectionOverlap(const Projection &p1, const Projection &p2) {

		bool b = p1.max >= p2.min;
		bool b2 = p2.max >= p1.min;

		return b && b2;
	}

}