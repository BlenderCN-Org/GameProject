
set( PUB_INC

	include/AssetLib/AssetLib.hpp
	include/AssetLib/ModelStructs.hpp
	
	)

set( SRC

	src/FileLoader.cpp
	src/LoaderHelpers.hpp
	src/ModelLoader.cpp
	src/ModelStructs.cpp
	src/ObjLoader.cpp
	src/TextureLoader.cpp
	src/Tinyxml.cpp
		
	src/Collada/ColladaLoader.cpp
	src/Collada/ColladaLoader.hpp
	
	src/CustomMesh/MeshFormat.hpp
	
	src/Load/MeshReader.cpp
	
	src/Save/MeshWriter.cpp
	src/Save/WriteVeresion.hpp
	src/Save/WriteVersion_2_0.cpp
	src/Save/WriteVersion_2_1.cpp
) 

add_library(assetLib_lib STATIC ${SRC} ${PUB_INC})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC})
target_include_directories(assetLib_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(assetLib_lib PRIVATE ${GLM_INCLUDE_DIR})
target_include_directories(assetLib_lib PRIVATE ${STB_INCLUDE_DIRS})
add_dependencies(assetLib_lib glm)