
set( PUB_INC

	include/Network/NetSocket.hpp
	
	)

set( SRC

	src/OsSocket.hpp
	src/NetSocket_Win.cpp
	src/NetSocket_Unix.cpp
	
) 

add_library(netSocket_lib STATIC ${SRC} ${PUB_INC})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC})
target_include_directories(netSocket_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)