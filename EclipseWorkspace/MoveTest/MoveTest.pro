TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -Wno-unused-parameter

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

SOURCES += \
    MoveTest.cpp

QMAKE_CLEAN += $$DESTDIR/$$TARGET*

HEADERS +=
