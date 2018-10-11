
if(MAKE_SERVER)
	add_subdirectory(server)
endif()

if(MAKE_CLIENT)
	add_subdirectory(gameProject)
endif()