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

QMAKE_CXXFLAGS += -Wno-unused-result -Wno-unused-parameter
QMAKE_CFLAGS += -Wno-unused-result -Wno-maybe-uninitialized
SOURCES += \
    TokensTest.cpp

QMAKE_CLEAN += $$DESTDIR/$$TARGET
