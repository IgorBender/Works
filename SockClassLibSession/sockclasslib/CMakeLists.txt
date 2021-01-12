cmake_minimum_required(VERSION 3.5)

project(sockclasslib LANGUAGES CXX)


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
)

