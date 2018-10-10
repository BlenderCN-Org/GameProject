
set (SRC

	src/main.cpp
	
	)
	
add_executable(server ${SRC})
target_link_libraries(server PRIVATE config_lib core_lib physicsEngine_lib assetLib_lib netSocket_lib)

if("${SYSTEM}" STREQUAL "WINDOWS_X64")
	message("Adding windows libraries")	
	target_link_libraries(server PRIVATE wsock32 ws2_32)
endif()

target_include_directories(server PRIVATE ${GLM_INCLUDE_DIR})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC})
set_target_properties(server PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

if(MAKE_SERVER)
	target_compile_definitions(server PRIVATE OPT_SERVER=1)
else()
	target_compile_definitions(server PRIVATE OPT_SERVER=0)
endif()