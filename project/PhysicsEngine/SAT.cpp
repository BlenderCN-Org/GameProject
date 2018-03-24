#include "SAT.hpp"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/projection.hpp>

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

	Dist projectAABB(const AABBShape& shape, const Axis& a) {

		Dist d;

		glm::vec3 hs = shape.halfSize;

		glm::vec3 p1 = shape.center + glm::vec3(+hs.x, +hs.y, +hs.z);
		glm::vec3 p2 = shape.center + glm::vec3(+hs.x, +hs.y, -hs.z);
		glm::vec3 p3 = shape.center + glm::vec3(+hs.x, -hs.y, +hs.z);
		glm::vec3 p4 = shape.center + glm::vec3(+hs.x, -hs.y, -hs.z);

		glm::vec3 p5 = shape.center + glm::vec3(-hs.x, +hs.y, +hs.z);
		glm::vec3 p6 = shape.center + glm::vec3(-hs.x, +hs.y, -hs.z);
		glm::vec3 p7 = shape.center + glm::vec3(-hs.x, -hs.y, +hs.z);
		glm::vec3 p8 = shape.center + glm::vec3(-hs.x, -hs.y, -hs.z);

		p1 = project(p1, a);
		p2 = project(p2, a);
		p3 = project(p3, a);
		p4 = project(p4, a);
		p5 = project(p5, a);
		p6 = project(p6, a);
		p7 = project(p7, a);
		p8 = project(p8, a);

		d.update(p1);
		d.update(p2);
		d.update(p3);
		d.update(p4);
		d.update(p5);
		d.update(p6);
		d.update(p7);
		d.update(p8);

		return d;
	}

	Dist projectOOBB(const OOBBShape& shape, const Axis& a) {

		Dist d;

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

		p1 = project(p1, a);
		p2 = project(p2, a);
		p3 = project(p3, a);
		p4 = project(p4, a);
		p5 = project(p5, a);
		p6 = project(p6, a);
		p7 = project(p7, a);
		p8 = project(p8, a);

		d.update(p1);
		d.update(p2);
		d.update(p3);
		d.update(p4);
		d.update(p5);
		d.update(p6);
		d.update(p7);
		d.update(p8);

		return d;
	}

	bool isPointOnLine(glm::vec3 p, Dist line)
	{
		glm::vec3 l = glm::isnan(line.max);
		glm::vec3 l2 = glm::isnan(line.min);
		bool isNan = (l.x && l.y && l.z) || (l2.x && l2.y && l2.z);

		/*
		x1 < x < x2, assuming x1 < x2, or
		y1 < y < y2, assuming y1 < y2, or
		z1 < z < z2, assuming z1 < z2
		*/

		bool x = ((line.min.x <= p.x) && (p.x <= line.max.x));
		bool y = ((line.min.y <= p.y) && (p.y <= line.max.y));
		bool z = ((line.min.z <= p.z) && (p.z <= line.max.z));

		return (x && y && z ) || isNan;


	}

}