#include "../pch.h"

#include <PhysicsEngine/SAT.hpp>

#include <iostream>


TEST(Physics, SatGetAxes_Test) {

	{
		printf("Sat get axes test 01\n");

		const SAT::Axis A1Expected = glm::vec3(0, 1, 0);
		const SAT::Axis A2Expected = glm::vec3(1, 0, 0);
		const SAT::Axis A3Expected = glm::vec3(0, 0, 1);

		OOBBShape shape;
		SAT::Axis a[3];

		shape.center = glm::vec3(0.0F);
		shape.halfSize = glm::vec3(0.5F);
		shape.rotation = glm::vec3(0.0F);

		SAT::getAxes(shape, a);

		EXPECT_EQ(a[0], A1Expected);
		EXPECT_EQ(a[1], A2Expected);
		EXPECT_EQ(a[2], A3Expected);
	}

	{
		printf("Sat get axes test 02\n");

		const SAT::Axis A1Expected = glm::vec3(0, 0, -1);
		const SAT::Axis A2Expected = glm::vec3(1, 0, 0);
		const SAT::Axis A3Expected = glm::vec3(0, 1, 0);

		OOBBShape shape;
		SAT::Axis a[3];

		shape.center = glm::vec3(0.0F);
		shape.halfSize = glm::vec3(0.5F);
		shape.rotation = glm::vec3(90.0F, 0.0F, 0.0F);

		SAT::getAxes(shape, a);

		
		EXPECT_NEAR(a[0].x, A1Expected.x, FLT_EPSILON);
		EXPECT_NEAR(a[0].y, A1Expected.y, FLT_EPSILON);
		EXPECT_NEAR(a[0].z, A1Expected.z, FLT_EPSILON);
		EXPECT_NEAR(a[1].x, A2Expected.x, FLT_EPSILON);
		EXPECT_NEAR(a[1].y, A2Expected.y, FLT_EPSILON);
		EXPECT_NEAR(a[1].z, A2Expected.z, FLT_EPSILON);
		EXPECT_NEAR(a[2].x, A3Expected.x, FLT_EPSILON);
		EXPECT_NEAR(a[2].y, A3Expected.y, FLT_EPSILON);
		EXPECT_NEAR(a[2].z, A3Expected.z, FLT_EPSILON);
	}

	{
		printf("Sat get axes test 03\n");

		const SAT::Axis A1Expected = glm::vec3(0, 1, 0);
		const SAT::Axis A2Expected = glm::vec3(0, 0, 1);
		const SAT::Axis A3Expected = glm::vec3(-1, 0, 0);

		OOBBShape shape;
		SAT::Axis a[3];

		shape.center = glm::vec3(0.0F);
		shape.halfSize = glm::vec3(0.5F);
		shape.rotation = glm::vec3(0.0F, 90.0F, 0.0F);

		SAT::getAxes(shape, a);


		EXPECT_NEAR(a[0].x, A1Expected.x, FLT_EPSILON);
		EXPECT_NEAR(a[0].y, A1Expected.y, FLT_EPSILON);
		EXPECT_NEAR(a[0].z, A1Expected.z, FLT_EPSILON);
		EXPECT_NEAR(a[1].x, A2Expected.x, FLT_EPSILON);
		EXPECT_NEAR(a[1].y, A2Expected.y, FLT_EPSILON);
		EXPECT_NEAR(a[1].z, A2Expected.z, FLT_EPSILON);
		EXPECT_NEAR(a[2].x, A3Expected.x, FLT_EPSILON);
		EXPECT_NEAR(a[2].y, A3Expected.y, FLT_EPSILON);
		EXPECT_NEAR(a[2].z, A3Expected.z, FLT_EPSILON);
	}

	{
		printf("Sat get axes test 04\n");

		const SAT::Axis A1Expected = glm::vec3(1, 0, 0);
		const SAT::Axis A2Expected = glm::vec3(0, -1, 0);
		const SAT::Axis A3Expected = glm::vec3(0, 0, 1);

		OOBBShape shape;
		SAT::Axis a[3];

		shape.center = glm::vec3(0.0F);
		shape.halfSize = glm::vec3(0.5F);
		shape.rotation = glm::vec3(0.0F, 0.0F, 90.0F);

		SAT::getAxes(shape, a);


		EXPECT_NEAR(a[0].x, A1Expected.x, FLT_EPSILON);
		EXPECT_NEAR(a[0].y, A1Expected.y, FLT_EPSILON);
		EXPECT_NEAR(a[0].z, A1Expected.z, FLT_EPSILON);
		EXPECT_NEAR(a[1].x, A2Expected.x, FLT_EPSILON);
		EXPECT_NEAR(a[1].y, A2Expected.y, FLT_EPSILON);
		EXPECT_NEAR(a[1].z, A2Expected.z, FLT_EPSILON);
		EXPECT_NEAR(a[2].x, A3Expected.x, FLT_EPSILON);
		EXPECT_NEAR(a[2].y, A3Expected.y, FLT_EPSILON);
		EXPECT_NEAR(a[2].z, A3Expected.z, FLT_EPSILON);
	}

	{
		printf("Sat get axes test 05\n");

		const SAT::Axis A1Expected = glm::vec3(0, -1, 0);
		const SAT::Axis A2Expected = glm::vec3(1, 0, 0);
		const SAT::Axis A3Expected = glm::vec3(0, 0, -1);

		OOBBShape shape;
		SAT::Axis a[3];

		shape.center = glm::vec3(0.0F);
		shape.halfSize = glm::vec3(0.5F);
		shape.rotation = glm::vec3(180.0F, 0.0F, 0.0F);

		SAT::getAxes(shape, a);


		EXPECT_NEAR(a[0].x, A1Expected.x, FLT_EPSILON);
		EXPECT_NEAR(a[0].y, A1Expected.y, FLT_EPSILON);
		EXPECT_NEAR(a[0].z, A1Expected.z, FLT_EPSILON);
		EXPECT_NEAR(a[1].x, A2Expected.x, FLT_EPSILON);
		EXPECT_NEAR(a[1].y, A2Expected.y, FLT_EPSILON);
		EXPECT_NEAR(a[1].z, A2Expected.z, FLT_EPSILON);
		EXPECT_NEAR(a[2].x, A3Expected.x, FLT_EPSILON);
		EXPECT_NEAR(a[2].y, A3Expected.y, FLT_EPSILON);
		EXPECT_NEAR(a[2].z, A3Expected.z, FLT_EPSILON);
	}
}

TEST(Physics, PointProject)
{
	{
		printf("Project point to axis 01\n");

		const glm::vec3 expectedPoint(0.0F);

		glm::vec3 axis = glm::vec3(0.0F, 1.0F, 0.0F);

		glm::vec3 result = SAT::project(glm::vec3(0, 0, 0), axis);

		EXPECT_NEAR(result.x, expectedPoint.x, FLT_EPSILON);
		EXPECT_NEAR(result.y, expectedPoint.y, FLT_EPSILON);
		EXPECT_NEAR(result.z, expectedPoint.z, FLT_EPSILON);
	}

	{
		printf("Project point to axis 02\n");

		const glm::vec3 expectedPoint(0.0F, 5.0F, 0.0F);

		glm::vec3 axis = glm::vec3(0.0F, 1.0F, 0.0F);

		glm::vec3 result = SAT::project(glm::vec3(20.0F, 5.0F, 0.0F), axis);

		EXPECT_NEAR(result.x, expectedPoint.x, FLT_EPSILON);
		EXPECT_NEAR(result.y, expectedPoint.y, FLT_EPSILON);
		EXPECT_NEAR(result.z, expectedPoint.z, FLT_EPSILON);
	}

	{
		printf("Project point to axis 03\n");

		const glm::vec3 expectedPoint(0.0F, 5.0F, 0.0F);

		glm::vec3 axis = glm::normalize(glm::vec3(1.0F, 1.0F, 0.0F));

		glm::vec3 result = SAT::project(glm::vec3(20.0F, 5.0F, 0.0F), axis);

		EXPECT_NEAR(result.x, expectedPoint.x, FLT_EPSILON);
		EXPECT_NEAR(result.y, expectedPoint.y, FLT_EPSILON);
		EXPECT_NEAR(result.z, expectedPoint.z, FLT_EPSILON);
	}

}

TEST(Physics, IsPointOnLine)
{
	{
		printf("Is point on line 01\n");

		SAT::Dist line;
		line.min.x = -1;
		line.min.y = 0;
		line.min.z = 0;

		line.max.x = 1;
		line.max.y = 0;
		line.max.z = 0;

		glm::vec3 point;

		EXPECT_TRUE(SAT::isPointOnLine(point, line));
	}

	{
		printf("Is point on line 02\n");

		SAT::Dist line;
		line.min.x = -1;
		line.min.y = 0;
		line.min.z = 0;

		line.max.x = 1;
		line.max.y = 0;
		line.max.z = 0;

		glm::vec3 point(0, 1, 0);

		EXPECT_FALSE(SAT::isPointOnLine(point, line));
	}

	{
		printf("Is point on line 03\n");

		SAT::Dist line;
		line.min.x = -1;
		line.min.y = -1;
		line.min.z = -1;

		line.max.x = 1;
		line.max.y = 1;
		line.max.z = 1;

		glm::vec3 point(0, 0, 0);

		EXPECT_TRUE(SAT::isPointOnLine(point, line));
	}

	{
		printf("Is point on line 04\n");

		SAT::Dist line;
		line.min.x = -1;
		line.min.y = -1;
		line.min.z = -1;

		line.max.x = 1;
		line.max.y = 1;
		line.max.z = 1;

		glm::vec3 point(0, 1, 0);

		EXPECT_FALSE(SAT::isPointOnLine(point, line));
	}
}