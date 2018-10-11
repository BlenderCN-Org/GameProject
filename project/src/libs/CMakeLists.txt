

add_subdirectory(AssetLib)
add_subdirectory(Config)
add_subdirectory(Network)
add_subdirectory(Core)

if(MAKE_CLIENT)
	add_subdirectory(RenderEngine)
	add_subdirectory(PhysicsEngine)
endif()