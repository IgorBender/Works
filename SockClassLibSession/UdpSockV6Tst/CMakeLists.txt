cmake_minimum_required(VERSION 3.5)

project(UdpSockV6Tst LANGUAGES CXX)


include_directories(../sockclasslib)
link_directories(${LINKDIR} )

add_executable(UdpSockV6Tst UdpSockV6Tst.cpp)

target_link_libraries(UdpSockV6Tst sockclasslib)

