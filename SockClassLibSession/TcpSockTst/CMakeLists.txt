cmake_minimum_required(VERSION 3.5)

project(TcpSockTst LANGUAGES CXX)


include_directories(../sockclasslib)
link_directories(${LINKDIR} )

add_executable(TcpSockTst TcpSockTst.cpp)

target_link_libraries(TcpSockTst sockclasslib)

