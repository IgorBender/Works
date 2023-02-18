cmake_minimum_required(VERSION 3.5)

project(TcpSockUdsTst LANGUAGES CXX)


include_directories(../sockclasslib)
link_directories(${LINKDIR} )

add_executable(TcpSockUdsTst TcpSockUdsTst.cpp)

target_link_libraries(TcpSockUdsTst sockclasslib)

