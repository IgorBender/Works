#-------------------------------------------------
#
# Project created by QtCreator 2016-10-18T05:40:11
#
#-------------------------------------------------

QT       -= core gui

TARGET = sockclasslib
TEMPLATE = lib

DEFINES += SOCKCLASSLIB_LIBRARY


CONFIG(debug_NO_EXCEPTIONS, debug|release|debug_NO_EXCEPTIONS|release_NO_EXCEPTIONS) {
    DESTDIR = ../../Debug_NO_EXCEPTIONS
    QMAKE_CXXFLAGS += -D_WITHOUT_SOCK_EXCEPTIONS
}
CONFIG(release_NO_EXCEPTIONS, debug|release|debug_NO_EXCEPTIONS|release_NO_EXCEPTIONS) {
    DESTDIR = ../../Release_NO_EXCEPTIONS
    QMAKE_CXXFLAGS += -D_WITHOUT_SOCK_EXCEPTIONS
}
CONFIG(debug, debug|release|debug_NO_EXCEPTIONS|release_NO_EXCEPTIONS) {
    DESTDIR = ../../Debug
}
CONFIG(release, debug|release|debug_NO_EXCEPTIONS|release_NO_EXCEPTIONS) {
    DESTDIR = ../../Release
}

QMAKE_CXXFLAGS += -std=c++0x -I. -Wno-unused-parameter -Wno-undef

QMAKE_CLEAN += $$DESTDIR/lib$$TARGET*

VERSION = 3.0.6

SOURCES += \
    BoundSocketV4.cpp \
    BoundSocketV6.cpp \
    BroadcastBound.cpp \
    BroadcastSocket.cpp \
    ConnectedSocketV4.cpp \
    ConnectedSocketV6.cpp \
    DatagramBound.cpp \
    DatagramBoundV6.cpp \
    DatagramSocket.cpp \
    DatagramSocketV6.cpp \
    InternetSocketV4.cpp \
    InternetSocketV6.cpp \
    MulticastBound.cpp \
    MulticastBoundV6.cpp \
    MulticastSocket.cpp \
    MulticastSocketV6.cpp \
    ServerReuse.cpp \
    ServerReuseV6.cpp \
    ServerSimple.cpp \
    ServerSimpleV6.cpp \
    sockclasslib.cpp \
    SocketClass.cpp \
    SockException.cpp \
    StreamBoundV4.cpp \
    StreamBoundV6.cpp \
    StreamSocket.cpp \
    StreamSocketV4.cpp \
    StreamSocketV6.cpp

HEADERS +=\
    BaseException.h \
    BoundSocketV4.h \
    BoundSocketV6.h \
    BroadcastBound.h \
    BroadcastSocket.h \
    ClientBound.h \
    ClientBoundV6.h \
    ClientSimple.h \
    ClientSimpleV6.h \
    ConnectedSocketV4.h \
    ConnectedSocketV6.h \
    DatagramBound.h \
    DatagramBoundV6.h \
    DatagramSocket.h \
    DatagramSocketV6.h \
    DisconnectException.h \
    InternetSocketV4.h \
    InternetSocketV6.h \
    MulticastBound.h \
    MulticastBoundV6.h \
    MulticastSocket.h \
    MulticastSocketV6.h \
    ServerReuse.h \
    ServerReuseV6.h \
    ServerSimple.h \
    ServerSimpleV6.h \
    sockclasslib.h \
    SocketClass.h \
    SockException.h \
    StreamBoundV4.h \
    StreamBoundV6.h \
    StreamSocket.h \
    StreamSocketV4.h \
    StreamSocketV6.h

