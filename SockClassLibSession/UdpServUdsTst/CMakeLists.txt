cmake_minimum_required(VERSION 3.5)

project(UdpServUdsTst LANGUAGES CXX)


include_directories(../sockclasslib)
link_directories(${LINKDIR} )

add_executable(UdpServUdsTst UdpServUdsTst.cpp)

target_link_libraries(UdpServUdsTst sockclasslib)

