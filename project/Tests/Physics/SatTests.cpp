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

}