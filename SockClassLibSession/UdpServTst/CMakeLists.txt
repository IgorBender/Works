cmake_minimum_required(VERSION 3.5)

project(UdpServTst LANGUAGES CXX)


include_directories(../sockclasslib)
link_directories(${LINKDIR} )

add_executable(UdpServTst UdpServTst.cpp)

target_link_libraries(UdpServTst sockclasslib)

