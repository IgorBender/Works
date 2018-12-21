TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

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

INCLUDEPATH += ../sockclasslib

QMAKE_CXXFLAGS += -std=c++0x -I. -I../../sockclasslib -Wno-unused-parameter -Wno-undef
QMAKE_LFLAGS += -L$$DESTDIR -Xlinker -rpath=.

LIBS += -lsockclasslib

QMAKE_CLEAN += $$DESTDIR/$$TARGET

SOURCES += \
    UdpSockTst.cpp
