cmake_minimum_required(VERSION 3.5)

project(UdpSockTst LANGUAGES CXX)


include_directories(../sockclasslib)
link_directories(${LINKDIR} )

add_executable(UdpSockTst UdpSockTst.cpp)

target_link_libraries(UdpSockTst sockclasslib)

