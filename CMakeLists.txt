cmake_minimum_required(VERSION 3.17)
project(vspg)

set(CMAKE_CXX_STANDARD 17)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)


link_directories(pugixml/src)
add_library(pugixml pugixml/src/pugixml.cpp)
link_libraries(pugixml)


include_directories(pugixml/src)
include_directories(src)

add_executable(vspg src/main.cpp)
add_executable(test src/test.cpp)
