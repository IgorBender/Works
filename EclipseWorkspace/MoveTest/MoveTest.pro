TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -Wno-unused-parameter

CONFIG(debug, debug|release) {
    DESTDIR = ../../Debug
}
CONFIG(release, debug|release) {
    DESTDIR = ../../Release
}


SOURCES += \
    MoveTest.cpp

QMAKE_CLEAN += $$DESTDIR/$$TARGET*

HEADERS +=
