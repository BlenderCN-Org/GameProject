

set( SRC
	
	src/LibraryLoader.cpp
	src/FormattedString.cpp
	src/String.cpp
	src/System.cpp
	
	include/Core/LibraryLoader.hpp
	include/Core/FormattedString.hpp
	include/Core/String.hpp
	include/Core/System.hpp
	include/Core/SafeQueue.hpp
	
	include/AssetHandling/AssetManager.cpp
	include/AssetHandling/AssetManager.hpp
	include/AssetHandling/IAssetManager.hpp
	
	include/AssetHandling/Loader/EditLoader.cpp
	include/AssetHandling/Loader/EditLoader.hpp
	include/AssetHandling/Loader/IEdit.hpp
	include/AssetHandling/Loader/IEditLoader.hpp
	include/AssetHandling/Loader/ILoader.hpp
	include/AssetHandling/Loader/MapLoader.cpp
	include/AssetHandling/Loader/MapLoader.hpp
	
) 

add_library(core_lib STATIC ${SRC} )
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC})
target_include_directories(core_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(core_lib PRIVATE ${GLM_INCLUDE_DIR})
target_include_directories(core_lib PRIVATE ${RENDER_ENGINE_IF})
add_dependencies(core_lib glm)