cmake_minimum_required(VERSION 3.5)

project(UdpServV6Tst LANGUAGES CXX)


include_directories(../sockclasslib)
link_directories(${LINKDIR} )

add_executable(UdpServV6Tst UdpServV6Tst.cpp)

target_link_libraries(UdpServV6Tst sockclasslib)

