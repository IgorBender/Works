cmake_minimum_required(VERSION 3.5)

project(StatesMachineTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

IF("${CMAKE_CXX_COMPILER}" STREQUAL "/usr/bin/clang++")
    IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../DebugClang)
    ENDIF()
    IF("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../ReleaseClang)
    ENDIF()
ENDIF()

IF("${CMAKE_CXX_COMPILER}" STREQUAL "/usr/bin/g++")
    IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../Debug)
    ENDIF()
    IF("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../Release)
    ENDIF()
ENDIF()

IF("${CMAKE_CXX_COMPILER}" STREQUAL "/home/opt/intel/sw_dev_tools/bin/icpc")
    IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../DebugIntel)
    ENDIF()
    IF("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../ReleaseIntel)
    ENDIF()
ENDIF()

add_executable(StatesMachineTest StatesMachineTest.cpp
    StateMachine.h  StatesTable.h  SystemState.h
    StateMachineTestClass.h StateMachineTestClass.cpp
    StringStateMachineTestClass.h StringStateMachineTestClass.cpp
    TestSystemStates.h TestSystemStates.cpp StringTestSystemStates.h StringTestSystemStates.cpp
    TestDefinitions.h
    )
