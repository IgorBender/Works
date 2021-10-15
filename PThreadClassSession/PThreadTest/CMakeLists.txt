cmake_minimum_required(VERSION 3.5)

project(PThreadTest LANGUAGES CXX)


add_executable(PThreadTest PThreadTest.cpp)

target_include_directories(PThreadTest PUBLIC ../PThreadClassLib)
target_link_directories(PThreadTest PUBLIC ${LINKDIR})

if(NOT UNIX)
    target_compile_definitions(PThreadTest PUBLIC _USE_MATH_DEFINES)
    target_link_libraries(PThreadTest PUBLIC PThreadClassLib pthreads)
else()
    target_link_libraries(PThreadTest PUBLIC PThreadClassLib pthread)
endif()
