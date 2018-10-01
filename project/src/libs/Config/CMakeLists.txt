
add_library(config_lib STATIC src/Config.cpp include/Config/Config.hpp)
target_include_directories(config_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
