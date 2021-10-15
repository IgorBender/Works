cmake_minimum_required(VERSION 3.5)

project(pthreads4w LANGUAGES C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(pthreads SHARED
     pthread.c pthread.h
)

target_compile_definitions(pthreads PRIVATE HAVE_CONFIG_H _CRT_SECURE_NO_WARNINGS)