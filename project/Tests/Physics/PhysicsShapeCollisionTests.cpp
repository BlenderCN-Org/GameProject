#include "../pch.h"

#include <PhysicsEngine/CollisionFuncs.hpp>
#include <PhysicsEngine/Shapes/PlaneShape.hpp>
#include <PhysicsEngine/Shapes/SphereShape.hpp>

#include <iostream>

TEST(Physics, PlaneVsSphere_Test) {

	{
		printf("Plane vs sphere test 1\n");
		PlaneShape ps;
		SphereShape ss;

		ps.distance = 0.0F;
		ps.normal = glm::vec3(0.0F, 1.0F, 0.0F);

		ss.center = glm::vec3(0.0F, 0.0F, 0.0F);
		ss.radius = 1.0F;

		EXPECT_TRUE(PlaneVsSphere(ps, ss));
	}

	{
		printf("Plane vs sphere test 2\n");
		PlaneShape ps;
		SphereShape ss;

		ps.distance = 0.0F;
		ps.normal = glm::vec3(0.0F, 1.0F, 0.0F);

		ss.center = glm::vec3(0.0F, 2.0F, 0.0F);
		ss.radius = 1.0F;

		EXPECT_FALSE(PlaneVsSphere(ps, ss));
	}

	{
		printf("Plane vs sphere test 3\n");
		PlaneShape ps;
		SphereShape ss;

		ps.distance = 0.0F;
		ps.normal = glm::vec3(0.0F, 1.0F, 0.0F);

		ss.center = glm::vec3(0.0F, -2.0F, 0.0F);
		ss.radius = 1.0F;

		EXPECT_FALSE(PlaneVsSphere(ps, ss));
	}
}

TEST(Physics, SphereVsSphere_Test) {

	{
		printf("Sphere vs sphere test 1\n");
		SphereShape ps;
		SphereShape ss;

		ps.center = glm::vec3(0.0F, 0.0F, 0.0F);
		ps.radius = 1.0F;

		ss.center = glm::vec3(0.0F, 0.0F, 0.0F);
		ss.radius = 1.0F;

		EXPECT_TRUE(SphereVsSphere(ps, ss));
	}

	{
		printf("Sphere vs sphere test 2\n");
		SphereShape ps;
		SphereShape ss;

		ps.center = glm::vec3(0.0F, 0.0F, 0.0F);
		ps.radius = 1.0F;

		ss.center = glm::vec3(0.0F, 2.0F, 0.0F);
		ss.radius = 1.0F;

		EXPECT_FALSE(SphereVsSphere(ps, ss));
	}

	{
		printf("Sphere vs sphere test 3\n");
		SphereShape ps;
		SphereShape ss;

		ps.center = glm::vec3(0.0F, 0.0F, 0.0F);
		ps.radius = 1.0F;

		ss.center = glm::vec3(0.0F, -2.0F, 0.0F);
		ss.radius = 1.0F;

		EXPECT_FALSE(SphereVsSphere(ps, ss));
	}
}

TEST(Physics, AABBVsAABB_Test) {

	{
		printf("AABB vs AABB test 1\n");
		AABBShape ps;
		AABBShape ss;

		ps.center = glm::vec3(0.0F, 0.0F, 0.0F);
		ps.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);

		ss.center = glm::vec3(0.0F, 0.0F, 0.0F);
		ss.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);

		EXPECT_TRUE(AABBVsAABB(ps, ss));
	}

	{
		printf("AABB vs AABB test 2\n");
		AABBShape ps;
		AABBShape ss;

		ps.center = glm::vec3(0.0F, 0.0F, 0.0F);
		ps.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);

		ss.center = glm::vec3(0.0F, 2.0F, 0.0F);
		ss.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);

		EXPECT_FALSE(AABBVsAABB(ps, ss));
	}

	{
		printf("AABB vs AABB test 3\n");
		AABBShape ps;
		AABBShape ss;

		ps.center = glm::vec3(0.0F, 0.0F, 0.0F);
		ps.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);

		ss.center = glm::vec3(0.0F, -2.0F, 0.0F);
		ss.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);

		EXPECT_FALSE(AABBVsAABB(ps, ss));
	}
}


TEST(Physics, AABBVsOOBB_Test) {

	{
		printf("AABB vs OOBB test 1\n");
		AABBShape ps;
		OOBBShape ss;

		ps.center = glm::vec3(0.0F, 0.0F, 0.0F);
		ps.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);

		ss.center = glm::vec3(0.0F, 0.0F, 0.0F);
		ss.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);
		ss.rotation = glm::vec3(0.0F);

		EXPECT_TRUE(AABBVsOOBB(ps, ss));
	}

	{
		printf("AABB vs OOBB test 2\n");
		AABBShape ps;
		OOBBShape ss;

		ps.center = glm::vec3(0.0F, 0.0F, 0.0F);
		ps.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);

		ss.center = glm::vec3(0.0F, 20.0F, 0.0F);
		ss.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);
		ss.rotation = glm::vec3(0.0F);

		EXPECT_FALSE(AABBVsOOBB(ps, ss));
	}

	{
		printf("AABB vs OOBB test 3\n");
		AABBShape ps;
		OOBBShape ss;

		ps.center = glm::vec3(0.0F, 0.0F, 0.0F);
		ps.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);

		ss.center = glm::vec3(0.0F, -2.0F, 0.0F);
		ss.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);
		ss.rotation = glm::vec3(0.0F);

		EXPECT_FALSE(AABBVsOOBB(ps, ss));
	}

	{
		printf("AABB vs OOBB test 4\n");
		AABBShape ps;
		OOBBShape ss;

		ps.center = glm::vec3(0.0F, 0.0F, 0.0F);
		ps.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);

		ss.center = glm::vec3(0.0F, -1.0F, 0.0F);
		ss.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);
		ss.rotation = glm::vec3(0.0F, 0.0F, 0.0F);

		EXPECT_TRUE(AABBVsOOBB(ps, ss));
	}

	{
		printf("AABB vs OOBB test 5\n");
		AABBShape ps;
		OOBBShape ss;

		ps.center = glm::vec3(0.0F, 0.0F, 0.0F);
		ps.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);

		ss.center = glm::vec3(0.0F, -1.0F, 0.0F);
		ss.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);
		ss.rotation = glm::vec3(45.0F, 0.0F, 0.0F);

		EXPECT_TRUE(AABBVsOOBB(ps, ss));
	}

}

TEST(Physics, OOBBVsOOBB_Test) {

	{
		printf("OOBB vs OOBB test 1\n");
		OOBBShape ps;
		OOBBShape ss;

		ps.center = glm::vec3(0.0F, 0.0F, 0.0F);
		ps.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);
		ps.rotation = glm::vec3(0.0F);

		ss.center = glm::vec3(0.0F, 0.0F, 0.0F);
		ss.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);
		ss.rotation = glm::vec3(0.0F);

		EXPECT_TRUE(OOBBVsOOBB(ps, ss));
	}

	{
		printf("OOBB vs OOBB test 2\n");
		OOBBShape ps;
		OOBBShape ss;

		ps.center = glm::vec3(0.0F, 0.0F, 0.0F);
		ps.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);
		ps.rotation = glm::vec3(0.0F);

		ss.center = glm::vec3(0.0F, 20.0F, 0.0F);
		ss.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);
		ss.rotation = glm::vec3(0.0F);

		EXPECT_FALSE(OOBBVsOOBB(ps, ss));
	}

	{
		printf("OOBB vs OOBB test 3\n");
		OOBBShape ps;
		OOBBShape ss;

		ps.center = glm::vec3(0.0F, 0.0F, 0.0F);
		ps.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);
		ps.rotation = glm::vec3(0.0F);

		ss.center = glm::vec3(0.0F, -2.0F, 0.0F);
		ss.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);
		ss.rotation = glm::vec3(0.0F);

		EXPECT_FALSE(OOBBVsOOBB(ps, ss));
	}

	{
		printf("OOBB vs OOBB test 4\n");
		OOBBShape ps;
		OOBBShape ss;

		ps.center = glm::vec3(0.0F, 0.0F, 0.0F);
		ps.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);
		ps.rotation = glm::vec3(0.0F);

		ss.center = glm::vec3(0.0F, -1.0F, 0.0F);
		ss.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);
		ss.rotation = glm::vec3(0.0F, 0.0F, 0.0F);

		EXPECT_TRUE(OOBBVsOOBB(ps, ss));
	}

	{
		printf("OOBB vs OOBB test 5\n");
		OOBBShape ps;
		OOBBShape ss;

		ps.center = glm::vec3(0.0F, 0.0F, 0.0F);
		ps.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);
		ps.rotation = glm::vec3(0.0F);

		ss.center = glm::vec3(0.0F, -1.0F, 0.0F);
		ss.halfSize = glm::vec3(0.5F, 0.5F, 0.5F);
		ss.rotation = glm::vec3(45.0F, 0.0F, 0.0F);

		EXPECT_TRUE(OOBBVsOOBB(ps, ss));
	}

}