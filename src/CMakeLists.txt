cmake_minimum_required(VERSION 3.00)

project(hnet)

SET(CMAKE_BUILD_TYPE "Release")
SET(CMAKE_PLATFORM_TYPE "Linux")

message("platform:"$ENV{COMPILEPLATFORM})

if (CMAKE_PLATFORM_TYPE STREQUAL "ArmLinux")
set(CMAKE_CXX_COMPILER arm-linux-g++)
set(CMAKE_C_COMPILER arm-linux-gcc)
add_definitions(-DARM_LINUX)
endif (CMAKE_PLATFORM_TYPE STREQUAL "ArmLinux")

if (CMAKE_PLATFORM_TYPE STREQUAL "ArmLinuxEABI")
set(CMAKE_CXX_COMPILER arm-none-linux-gnueabi-g++)
add_definitions(-DARM_LINUX)
endif (CMAKE_PLATFORM_TYPE STREQUAL "ArmLinuxEABI")

if (CMAKE_PLATFORM_TYPE STREQUAL "ArmLinuxEABIHF")
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
add_definitions(-DARM_LINUX)
endif (CMAKE_PLATFORM_TYPE STREQUAL "ArmLinuxEABIHF")

if (CMAKE_PLATFORM_TYPE STREQUAL "Linux")
set(CMAKE_CXX_COMPILER g++)
add_definitions(-DLINUX)
endif (CMAKE_PLATFORM_TYPE STREQUAL "Linux")

set(CXX_FLAGS_DEBUG -O0 -ggdb3 -Werror  -std=c++11 -Wall -Wno-strict-aliasing  -fPIC --warn-no-format)
set(CXX_FLAGS -O0 -Werror -Wall -Wshadow -std=c++11 -Wno-strict-aliasing  -fPIC --warn-no-format)

SET(LIBRARY_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/build/lib)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
add_definitions(-DDEBUG)
add_definitions(${CXX_FLAGS_DEBUG})
else (CMAKE_BUILD_TYPE STREQUAL "Debug")
add_definitions(${CXX_FLAGS})
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

add_definitions(-DCONFPATH="")
add_definitions(-DBINPATH="")

message( ${PROJECT_SOURCE_DIR})


message( "project bin dir:" ${PROJECT_BINARY_DIR} )
message( "project src dir:" ${PROJECT_SOURCE_DIR} )

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_BINARY_DIR}/bin)


add_executable(udpPFTest
    udpPfUnixTest.cc)

INSTALL(DIRECTORY ${PROJECT_BINARY_DIR}/bin/  DESTINATION  ./bin
USE_SOURCE_PERMISSIONS)
INSTALL(DIRECTORY ${PROJECT_BINARY_DIR}/lib/ DESTINATION  ./lib
USE_SOURCE_PERMISSIONS)


