cmake_minimum_required(VERSION 3.5)

project(UdpSockUdsTst LANGUAGES CXX)


include_directories(../sockclasslib)
link_directories(${LINKDIR} )

add_executable(UdpSockUdsTst UdpSockUdsTst.cpp)

target_link_libraries(UdpSockUdsTst sockclasslib)

