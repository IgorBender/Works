cmake_minimum_required(VERSION 3.5)

project(XsdTest LANGUAGES CXX)

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

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modules)
#find_program(XSD xsdcxx REQUIRED)
find_package(Xsd REQUIRED)
find_package(XercesC REQUIRED)


add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/XsdExample.cpp ${CMAKE_CURRENT_SOURCE_DIR}/XsdExample.h
    COMMAND ${XSDCXX_EXECUTABLE} cxx-tree --std c++11 --output-dir ${CMAKE_CURRENT_SOURCE_DIR}/ --namespace-map XsdExample.xsd=XsdExample
    --generate-serialization --hxx-suffix .h --cxx-suffix .cpp ${CMAKE_CURRENT_SOURCE_DIR}/XsdExample.xsd
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/XsdExample.xsd
    COMMENT "Generating XSD mapping..."
    VERBATIM)

link_libraries(xerces-c)

add_executable(XsdTest XsdTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/XsdExample.cpp ${CMAKE_CURRENT_SOURCE_DIR}/XsdExample.h)

