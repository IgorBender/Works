cmake_minimum_required(VERSION 3.5)

project(ThreadClassLib LANGUAGES CXX)

add_library(ThreadClassLib SHARED
ThreadClassLib.cpp ThreadClassLib.h
CondVarClass.cpp   CondVarClass.h
ThreadClass.cpp ThreadClass.h
ThreadException.cpp ThreadException.h
)

if( NOT UNIX)
    target_compile_definitions(ThreadClassLib PRIVATE THREADCLASSLIB_EXPORTS)
endif()

