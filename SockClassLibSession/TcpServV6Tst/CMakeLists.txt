cmake_minimum_required(VERSION 3.5)

project(TcpServV6Tst LANGUAGES CXX)


include_directories(../sockclasslib)
link_directories(${LINKDIR} )

add_executable(TcpServV6Tst TcpServV6Tst.cpp)

target_link_libraries(TcpServV6Tst sockclasslib)

