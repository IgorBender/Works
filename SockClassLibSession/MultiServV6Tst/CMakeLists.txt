cmake_minimum_required(VERSION 3.5)

project(MultiServV6Tst LANGUAGES CXX)


include_directories(../sockclasslib)
link_directories(${LINKDIR} )

add_executable(MultiServV6Tst MultiServV6Tst.cpp)

target_link_libraries(MultiServV6Tst sockclasslib)

