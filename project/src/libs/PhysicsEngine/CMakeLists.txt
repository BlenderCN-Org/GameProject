
set( PUB_INC

	include/PhysicsEngine/IPhysShape.hpp
	)

set( SRC

	src/CollisionFuncs.cpp
	src/CollisionFuncs.hpp
	src/PhysicsEngine.cpp
	src/PhysicsEngine.hpp
	src/SAT.hpp
	src/SAT.cpp
	src/RigidBody.hpp
	src/StaticObject.hpp
		
	src/Shapes/AABBShape.hpp
	src/Shapes/OOBBShape.hpp
	src/Shapes/PlaneShape.hpp
	src/Shapes/ShapeGroup.hpp
	src/Shapes/SphereShape.hpp
) 

add_library(physicsEngine_lib STATIC ${SRC} ${PUB_INC})
set_target_properties(physicsEngine_lib PROPERTIES OUTPUT_NAME "physicsEngine")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC})
target_include_directories(physicsEngine_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(physicsEngine_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(physicsEngine_lib PRIVATE ${GLM_INCLUDE_DIR})
add_dependencies(physicsEngine_lib glm)