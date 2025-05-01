cmake_minimum_required(VERSION 3.5)

project(ThreadsPoolTest LANGUAGES CXX)


include_directories(../PThreadClassLib)
link_directories(${LINKDIR} )

add_executable(ThreadsPoolTest ThreadsPoolTest.cpp
PoolThread.cpp    PThreadsPool.h           TrackingThreadsPool.h
PoolThread.h      PThreadsPool.cpp  TrackingThreadsPool.cpp
)

if(NOT UNIX)
    target_compile_definitions(ThreadsPoolTest PUBLIC _USE_MATH_DEFINES)
    target_link_libraries(ThreadsPoolTest PUBLIC PThreadClassLib pthreads)
else()
    target_link_libraries(ThreadsPoolTest PUBLIC PThreadClassLib pthread)
endif()

