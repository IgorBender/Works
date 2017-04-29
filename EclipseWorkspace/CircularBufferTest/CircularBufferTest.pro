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

QMAKE_CXXFLAGS += -Wno-unused-parameter

SOURCES += \
    CyclicBuffTest.cpp

HEADERS += \
    CircularBuffer.h

QMAKE_CLEAN += $$DESTDIR/$$TARGET*
