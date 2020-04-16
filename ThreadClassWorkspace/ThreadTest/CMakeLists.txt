cmake_minimum_required(VERSION 3.5)

project(ThreadTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

IF("${CMAKE_CXX_COMPILER}" STREQUAL "/usr/bin/clang++")
    IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../DebugClang)
        set(LINKDIR ${CMAKE_CURRENT_SOURCE_DIR}/../DebugClang)
    ENDIF()
    IF("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../ReleaseClang)
        set(LINKDIR ${CMAKE_CURRENT_SOURCE_DIR}/../ReleaseClang)
    ENDIF()
ENDIF()

IF("${CMAKE_CXX_COMPILER}" STREQUAL "/usr/bin/g++")
    IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../Debug)
        set(LINKDIR ${CMAKE_CURRENT_SOURCE_DIR}/../Debug)
    ENDIF()
    IF("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../Release)
        set(LINKDIR ${CMAKE_CURRENT_SOURCE_DIR}/../Release)
    ENDIF()
ENDIF()

IF("${CMAKE_CXX_COMPILER}" STREQUAL "/home/opt/intel/sw_dev_tools/bin/icpc")
    IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../DebugIntel)
        set(LINKDIR ${CMAKE_CURRENT_SOURCE_DIR}/../DebugIntel)
    ENDIF()
    IF("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../ReleaseIntel)
        set(LINKDIR ${CMAKE_CURRENT_SOURCE_DIR}/../ReleaseIntel)
    ENDIF()
ENDIF()

IF("${CMAKE_CXX_COMPILER}" STREQUAL "/home/opt/intel/sw_dev_tools/bin/icpc")
    set(CMAKE_SKIP_BUILD_RPATH false)
    set(CMAKE_SKIP_RPATH false)
    set(CMAKE_BUILD_RPATH "/home/opt/intel/sw_dev_tools/lib/intel64")
ENDIF()

include_directories(../ThreadClassLib)
link_directories(${LINKDIR} )

add_executable(ThreadTest ThreadTest.cpp)

target_link_libraries(ThreadTest ThreadClassLib pthread)

