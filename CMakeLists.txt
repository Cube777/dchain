cmake_minimum_required(VERSION 2.8)
project(dchain)

#Set build mode to release if no build-type was specified
if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release")
endif()

#Debug postfix
set(CMAKE_DEBUG_POSTFIX "d")

#Add C++11 support
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#Add include paths
include_directories(${CMAKE_SOURCE_DIR}/include)

#Add library modules
add_library(dchain SHARED "src/dchain.cpp")

#Tools for testing
add_executable(tools "${CMAKE_SOURCE_DIR}/tools.cpp")
target_link_libraries(tools dchain)

#Installation
file(GLOB includes "${CMAKE_SOURCE_DIR}/include/*.h")
install(FILES ${includes} DESTINATION "include/dchain")

set(INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib")

install(TARGETS dchain DESTINATION ${INSTALL_DIR})
