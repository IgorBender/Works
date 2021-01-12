cmake_minimum_required(VERSION 3.5)

project(MultiSockTst LANGUAGES CXX)


include_directories(../sockclasslib)
link_directories(${LINKDIR} )

add_executable(MultiSockTst MultiSockTst.cpp)

target_link_libraries(MultiSockTst sockclasslib)

