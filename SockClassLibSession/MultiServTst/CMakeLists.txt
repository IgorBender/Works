cmake_minimum_required(VERSION 3.5)

project(MultiServTst LANGUAGES CXX)


include_directories(../sockclasslib)
link_directories(${LINKDIR} )

add_executable(MultiServTst MultiServTst.cpp)

target_link_libraries(MultiServTst sockclasslib)

