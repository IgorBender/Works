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
QMAKE_RPATHDIR += /opt/intel/sw_dev_tools/lib/intel64
CONFIG(debug, debug|release) {
    DESTDIR = ../../DebugIntel
}
CONFIG(release, debug|release) {
    DESTDIR = ../../ReleaseIntel
}
}

LIBS += -lstdc++fs

SOURCES += \
        ScenOutTest.cpp

QMAKE_CLEAN += $$DESTDIR/$$TARGET
