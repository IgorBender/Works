#-------------------------------------------------
#
# Project created by QtCreator 2017-04-14T12:35:08
#
#-------------------------------------------------

QT       -= core gui

TARGET = sockclasslib

CONFIG(debug, debug|release) {
    DESTDIR = ../Debug
}
CONFIG(release, debug|release) {
    DESTDIR = ../Release
}

TEMPLATE = lib
VERSION = 3.0.6

DEFINES += SOCKCLASSLIB_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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
    ClientSimple.h \
    ClientSimpleV6.h \
    ConnectedSocketV4.h \
    ConnectedSocketV6.h \
    DatagramBound.h \
    DatagramBoundV6.h \
    DatagramSocket.h \
    DatagramSocketV6.h \
    DisconnectException.h \
    InternetSocket.h \
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

#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}

DEPENDPATH += .
QMAKE_CLEAN += $$DESTDIR/lib$$TARGET*
