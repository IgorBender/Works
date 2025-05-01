cmake_minimum_required(VERSION 3.5)

project(TcpServUdsTst LANGUAGES CXX)


include_directories(../sockclasslib)
link_directories(${LINKDIR} )

add_executable(TcpServUdsTst TcpServUdsTst.cpp)

target_link_libraries(TcpServUdsTst sockclasslib)

