cmake_minimum_required(VERSION 3.5)

project(TcpServTst LANGUAGES CXX)


include_directories(../sockclasslib)
link_directories(${LINKDIR} )

add_executable(TcpServTst TcpServTst.cpp)

target_link_libraries(TcpServTst sockclasslib)

