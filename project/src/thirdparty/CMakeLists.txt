
include(ExternalProject)

# glm
ExternalProject_Add(
    glm
    PREFIX ${CMAKE_BINARY_DIR}/glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
    )
	
ExternalProject_Get_Property(glm source_dir)
set(GLM_INCLUDE_DIR ${source_dir} CACHE STRING "" FORCE)

# glew
set(GLEW_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glew-2.1.0/include/")

if(NOT "${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
    set(GLEW_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/glew-2.1.0/lib/Release/Win32/glew32.lib")
else()
	set(GLEW_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/glew-2.1.0/lib/Release/x64/glew32.lib")
endif()


if("${SYSTEM}" STREQUAL "WINDOWS_X64")

	find_package(glew REQUIRED)

	set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIRS} CACHE STRING "" FORCE)
	set(GLEW_LIBRARIES ${GLEW_LIBRARIES} CACHE STRING "" FORCE)
	set(GLEW_FOUND ${GLEW_FOUND} CACHE BOOL "" FORCE)

endif()

# opengl
find_package(OpenGL REQUIRED)
set(OPENGL_INCLUDE_DIR ${OPENGL_INCLUDE_DIR} CACHE STRING "" FORCE)
set(OPENGL_LIBRARIES ${OPENGL_LIBRARIES} CACHE STRING "" FORCE)


# freetype (remove)
set(FREETYPE_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/FreeType/include/;${CMAKE_CURRENT_SOURCE_DIR}/FreeType/include/freetype/" CACHE STRING "" FORCE)
if(NOT "${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
    set(FREETYPE_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/FreeType/lib/Win32/freetype.lib" CACHE STRING "" FORCE)
else()
	set(FREETYPE_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/FreeType/lib/x64/freetype.lib" CACHE STRING "" FORCE)
endif()

# stb public library
set(STB_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/stb/include/" CACHE STRING "" FORCE)