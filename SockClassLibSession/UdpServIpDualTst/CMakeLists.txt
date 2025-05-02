cmake_minimum_required(VERSION 3.5)

project(UdpServIpDualTst LANGUAGES CXX)


include_directories(../sockclasslib)
link_directories(${LINKDIR} )

add_executable(UdpServIpDualTst UdpServIpDualTst.cpp)

target_link_libraries(UdpServIpDualTst sockclasslib)

