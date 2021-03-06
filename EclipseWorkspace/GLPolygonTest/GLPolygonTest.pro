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

QMAKE_CXXFLAGS += -Wno-unused-result
QMAKE_CFLAGS += -Wno-unused-result -Wno-maybe-uninitialized

SOURCES += \
    GLPolygonTest.cpp \
    GpcPolygon.cpp \
    gpc.c

HEADERS += \
    gpc.h \
    GpcPolygon.h

QMAKE_CLEAN += $$DESTDIR/$$TARGET*
LIBS += -lglut -lGLU -lGL -lXext -lX11
