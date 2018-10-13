

add_subdirectory(AssetLib)
add_subdirectory(Config)
add_subdirectory(Network)
add_subdirectory(Core)
add_subdirectory(PhysicsEngine)

if(MAKE_CLIENT)
	add_subdirectory(RenderEngine)	
endif()