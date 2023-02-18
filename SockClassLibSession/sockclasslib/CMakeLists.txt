cmake_minimum_required(VERSION 3.5)

project(sockclasslib LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

IF(UNIX)
set(FILES_LIST_UDS   UnixDomainSocket.h     UnixDomainSocket.cpp
    BoundSocketUds.h       BoundSocketUds.cpp
    ConnectedSocketUds.cpp ConnectedSocketUds.h
    StreamSocketUds.h      StreamSocketUds.cpp
    DatagramSocketUds.cpp  DatagramSocketUds.h
    DatagramBoundUds.cpp   DatagramBoundUds.h
    ClientSimpleUds.h      ClientBoundUds.h
    StreamBoundUds.h       StreamBoundUds.cpp
    ServerSimpleUds.h      ServerSimpleUds.cpp
)
ELSE()
set(FILES_LIST_UDS )
ENDIF()

add_library(sockclasslib SHARED
    BaseException.h        DatagramSocket.cpp     ServerSimple.cpp
    BoundSocketV4.cpp      DatagramSocket.h       ServerSimple.h
    BoundSocketV4.h        DatagramSocketV6.cpp   ServerSimpleV6.cpp
    BoundSocketV6.cpp      DatagramSocketV6.h     ServerSimpleV6.h
    BoundSocketV6.h        DisconnectException.h  sockclasslib.cpp
    BroadcastBound.cpp     InternetSocketV4.cpp   sockclasslib.h
    BroadcastBound.h       InternetSocketV4.h     SocketClass.cpp
    BroadcastSocket.cpp    InternetSocketV6.cpp   SocketClass.h
    BroadcastSocket.h      InternetSocketV6.h     SockException.cpp
    ClientBound.h          MulticastBound.cpp     SockException.h
    ClientBoundV6.h        MulticastBound.h       StreamBoundV4.cpp
    ClientSimple.h         MulticastBoundV6.cpp   StreamBoundV4.h
    ClientSimpleV6.h       MulticastBoundV6.h     StreamBoundV6.cpp
    ConnectedSocketV4.cpp  MulticastSocket.cpp    StreamBoundV6.h
    ConnectedSocketV4.h    MulticastSocket.h      StreamSocket.cpp
    ConnectedSocketV6.cpp  MulticastSocketV6.cpp  StreamSocket.h
    ConnectedSocketV6.h    MulticastSocketV6.h    StreamSocketV4.cpp
    DatagramBound.cpp      ServerReuse.cpp        StreamSocketV4.h
    DatagramBound.h        ServerReuse.h          StreamSocketV6.cpp
    DatagramBoundV6.cpp    ServerReuseV6.cpp      StreamSocketV6.h
    DatagramBoundV6.h      ServerReuseV6.h
    ${FILES_LIST_UDS}
)

IF(NOT UNIX)
    target_compile_definitions(sockclasslib PRIVATE SOCKCLASSLIB_EXPORTS _WINSOCK_DEPRECATED_NO_WARNINGS _CRT_SECURE_NO_WARNINGS)
    target_link_libraries(sockclasslib ws2_32 Iphlpapi)
ENDIF()
