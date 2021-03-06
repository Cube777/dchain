cmake_minimum_required(VERSION 2.8)
project(dchain)

#Set build mode to release if no build-type was specified
if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release")
endif()

#Debug postfix
set(CMAKE_DEBUG_POSTFIX "d")

#Add C++11 support
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

#Add include paths
include_directories(${CMAKE_SOURCE_DIR}/include)

#Add library modules
add_library(dchain SHARED "src/dchain.cpp")
add_library(dchain-bin SHARED "src/dchain-bin.cpp")
add_library(dchain-bin-threaded SHARED "src/dchain-bin-threaded.cpp")

#Tools for testing
add_executable(string_test "${CMAKE_SOURCE_DIR}/tools/string-test.cpp")
target_link_libraries(string_test dchain)
add_executable(threaded_speed "${CMAKE_SOURCE_DIR}/tools/threaded-speed.cpp")
target_link_libraries(threaded_speed dchain-bin-threaded)

#Installation
file(GLOB includes "${CMAKE_SOURCE_DIR}/include/*.h")
install(FILES ${includes} DESTINATION "include/dchain")

set(INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib")

install(TARGETS dchain dchain-bin dchain-bin-threaded DESTINATION ${INSTALL_DIR})
