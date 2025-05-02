cmake_minimum_required(VERSION 3.16)

project(SockClassLibCMake)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if(UNIX)
  # add_compile_definitions(_WITHOUT_SOCK_EXCEPTIONS)
else()
  add_compile_definitions(
    _CRT_SECURE_NO_WARNINGS _WINSOCK_DEPRECATED_NO_WARNINGS _USE_MATH_DEFINES
    _WITHOUT_SOCK_EXCEPTIONS)
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/Debug)
endif()
if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE
      ${CMAKE_CURRENT_SOURCE_DIR}/Release)
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  set(LINKDIR ${${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}})
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  set(LINKDIR ${${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}})
endif()

set(CMAKE_SKIP_BUILD_RPATH false)
set(CMAKE_SKIP_RPATH false)

add_subdirectory(sockclasslib)
add_subdirectory(MultiServTst)
add_subdirectory(MultiServV6Tst)
add_subdirectory(MultiSockTst)
add_subdirectory(MultiSockV6Tst)
add_subdirectory(TcpServTst)
add_subdirectory(TcpServV6Tst)
add_subdirectory(TcpSockTst)
add_subdirectory(TcpSockV6Tst)
add_subdirectory(UdpServTst)
add_subdirectory(UdpServV6Tst)
add_subdirectory(UdpSockTst)
add_subdirectory(UdpSockV6Tst)
add_subdirectory(TcpServIpDualTst)
add_subdirectory(UdpServIpDualTst)
if(UNIX)
  add_subdirectory(TcpServUdsTst)
  add_subdirectory(TcpSockUdsTst)
  add_subdirectory(UdpServUdsTst)
  add_subdirectory(UdpSockUdsTst)
endif()
