cmake_minimum_required(VERSION 3.5)

project(TcpServIpDualTst LANGUAGES CXX)


include_directories(../sockclasslib)
link_directories(${LINKDIR} )

add_executable(TcpServIpDualTst TcpServIpDualTst.cpp)

target_link_libraries(TcpServIpDualTst sockclasslib)

