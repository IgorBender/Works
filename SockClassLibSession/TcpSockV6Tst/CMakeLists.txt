cmake_minimum_required(VERSION 3.5)

project(TcpSockV6Tst LANGUAGES CXX)


include_directories(../sockclasslib)
link_directories(${LINKDIR} )

add_executable(TcpSockV6Tst TcpSockV6Tst.cpp)

target_link_libraries(TcpSockV6Tst sockclasslib)

