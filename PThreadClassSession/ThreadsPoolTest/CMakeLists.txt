cmake_minimum_required(VERSION 3.5)

project(ThreadsPoolTest LANGUAGES CXX)


include_directories(../PThreadClassLib)
link_directories(${LINKDIR} )

add_executable(ThreadsPoolTest ThreadsPoolTest.cpp
PoolThread.cpp    PThreadsPool.h           TrackingThreadsPool.h
PoolThread.h      PThreadsPool.cpp  TrackingThreadsPool.cpp
)

target_link_libraries(ThreadsPoolTest PThreadClassLib pthread)

