TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

CONFIG(debug, debug|release) {
    DESTDIR = ../Debug
}
CONFIG(release, debug|release) {
    DESTDIR = ../Release
}

#QMAKE_LFLAGS += -L$$DESTDIR
QMAKE_RPATHDIR += $$DESTDIR
INCLUDEPATH += ../sockclasslib
DEPENDPATH += $$INCLUDEPATH

SOURCES += \
    UdpSockTst.cpp

LIBS += -L$$DESTDIR -lsockclasslib
QMAKE_CLEAN += $$DESTDIR/$$TARGET*
