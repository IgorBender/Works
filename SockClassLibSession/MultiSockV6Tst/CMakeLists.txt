cmake_minimum_required(VERSION 3.5)

project(MultiSockV6Tst LANGUAGES CXX)


include_directories(../sockclasslib)
link_directories(${LINKDIR} )

add_executable(MultiSockV6Tst MultiSockV6Tst.cpp)

target_link_libraries(MultiSockV6Tst sockclasslib)

