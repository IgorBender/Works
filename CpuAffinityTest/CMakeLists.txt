cmake_minimum_required(VERSION 3.16)

project(CpuAffinityTest LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
#set(CMAKE_VERBOSE_MAKEFILE ON)

IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/Debug)
ELSE()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/Release)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_CURRENT_SOURCE_DIR}/Release)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/Release)
ENDIF()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})

set(CMAKE_SKIP_BUILD_RPATH false)
set(CMAKE_SKIP_RPATH false)

#IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
#        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/Debug)
#ELSE()
#        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/Release)
#ENDIF()

add_subdirectory(PThreadClassLib)

add_executable(CpuAffinityTest CpuAffinityTest.cpp TimingTest.h TimingTest.cpp AccumulatedAverage.h)

target_include_directories(CpuAffinityTest PUBLIC PThreadClassLib)
target_link_libraries(CpuAffinityTest PThreadClassLib pthread)

