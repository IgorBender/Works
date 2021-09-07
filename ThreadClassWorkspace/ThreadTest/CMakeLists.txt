cmake_minimum_required(VERSION 3.5)

project(ThreadTest LANGUAGES CXX)

add_executable(ThreadTest ThreadTest.cpp)
target_include_directories(ThreadTest PRIVATE ../ThreadClassLib)
target_link_directories(ThreadTest PRIVATE ${LINKDIR} )

if( NOT (${UNIX}))
    target_compile_definitions(ThreadClassLib PRIVATE THREADCLASSLIB_EXPORTS)
    target_link_libraries(ThreadTest ThreadClassLib)
else()
    target_link_libraries(ThreadTest ThreadClassLib pthread)
endif()

