cmake_minimum_required(VERSION 3.5)

project(PThreadTest LANGUAGES CXX)


include_directories(../PThreadClassLib)
link_directories(${LINKDIR} )

add_executable(PThreadTest PThreadTest.cpp)

target_link_libraries(PThreadTest PThreadClassLib pthread)

