
set( PUB_INC

	include/RenderEngine/IAnimatedMesh.hpp
	include/RenderEngine/IBuffer.hpp
	include/RenderEngine/IFont.hpp
	include/RenderEngine/IFrameBuffer.hpp
	include/RenderEngine/IMaterial.hpp
	include/RenderEngine/IMesh.hpp
	include/RenderEngine/IPixelBuffer.hpp
	include/RenderEngine/IRenderEngine.hpp
	include/RenderEngine/IShaderObject.hpp
	include/RenderEngine/ITexture.hpp
	include/RenderEngine/IWindow.hpp
	)

set( SRC

	src/BuildOptions.hpp
	src/RenderEngine.cpp
	src/RenderEngine.hpp
	src/ReGlobal.cpp
	src/ReGlobal.hpp
	
	src/Assets/AnimatedMesh_gl.hpp
	src/Assets/AnimatedMesh_gl.cpp
	src/Assets/Buffer_gl.hpp
	src/Assets/Buffer_gl.cpp
	src/Assets/Font_gl.hpp
	src/Assets/Font_gl.cpp
	src/Assets/FrameBuffer_gl.hpp
	src/Assets/FrameBuffer_gl.cpp
	src/Assets/Material_gl.hpp
	src/Assets/Material_gl.cpp
	src/Assets/Mesh_gl.hpp
	src/Assets/Mesh_gl.cpp
	src/Assets/Mesh_vk.hpp
	src/Assets/Mesh_vk.cpp
	src/Assets/PixelBuffer_gl.hpp
	src/Assets/PixelBuffer_gl.cpp
	src/Assets/ShaderObject_gl.hpp
	src/Assets/ShaderObject_gl.cpp
	src/Assets/Texture_gl.hpp
	src/Assets/Texture_gl.cpp
	src/Assets/ThreadLock.hpp
	src/Assets/ThreadLock.cpp
	
	src/DataTypes/Colors.hpp
	src/DataTypes/Colors.cpp
	src/DataTypes/ShaderNodes.hpp
	src/DataTypes/Vertex.hpp
	src/DataTypes/Vertex.cpp
	
	src/Utils/ShelfPack.hpp
	src/Utils/ShelfPack.cpp
	
	src/Window/BaseWindow.cpp
	src/Window/InputHandling.hpp
	src/Window/InputHandling.cpp
	src/Window/InputHelper.hpp
	src/Window/InputHelper.cpp
	src/Window/Win32Window.cpp
	src/Window/Window.hpp
	src/Window/WindowSystem.hpp
	src/Window/VKWindow.cpp
	src/Window/VulkanWindowHelper.cpp
	src/Window/VulkanWindowHelper.hpp
) 

add_library(renderEngine_lib SHARED ${SRC} ${PUB_INC})
set(RENDER_ENGINE_IF ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE STRING "" FORCE)
set_target_properties(renderEngine_lib PROPERTIES OUTPUT_NAME "renderEngine")
target_link_libraries(renderEngine_lib PRIVATE ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${FREETYPE_LIBRARIES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC})
target_include_directories(renderEngine_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(renderEngine_lib PRIVATE ${GLM_INCLUDE_DIR})
target_include_directories(renderEngine_lib PRIVATE ${GLEW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS})
add_dependencies(renderEngine_lib glm)

message("${FREETYPE_LIBRARIES}")
