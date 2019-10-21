TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

contains(QMAKE_CXX, clang++) {
CONFIG(debug, debug|release) {
    DESTDIR = ../../DebugClang
}
CONFIG(release, debug|release) {
    DESTDIR = ../../ReleaseClang
}
}

contains(QMAKE_CXX, g++) {
CONFIG(debug, debug|release) {
    DESTDIR = ../../Debug
}
CONFIG(release, debug|release) {
    DESTDIR = ../../Release
}
}

contains(QMAKE_CXX, icpc) {
QMAKE_RPATHDIR += /opt/intel/system_studio_2019/compilers_and_libraries_2019.5.281/linux/compiler/lib/intel64_lin
CONFIG(debug, debug|release) {
    DESTDIR = ../../DebugIntel
}
CONFIG(release, debug|release) {
    DESTDIR = ../../ReleaseIntel
}
}

LIBS += -lglut -lGL

SOURCES += \
        GLPolygonTest.cc \
        GenException.cpp \
        GpcPolygon.cpp \
        gpc.c

HEADERS += \
    GenException.h \
    GpcPolygon.h \
    gpc.h

QMAKE_CLEAN = $$DESTDIR/$$TARGET
