

add_subdirectory(AssetLib)
add_subdirectory(Config)
add_subdirectory(Network)

if(MAKE_CLIENT)
	add_subdirectory(RenderEngine)
	add_subdirectory(Core)
	add_subdirectory(PhysicsEngine)
endif()