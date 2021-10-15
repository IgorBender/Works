TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

contains(QMAKE_CXX, clang++) {
CONFIG(debug_NO_EXCEPTIONS, debug|release|debug_NO_EXCEPTIONS|release_NO_EXCEPTIONS) {
    DESTDIR = ../../DebugClang_NO_EXCEPTIONS
    QMAKE_CXXFLAGS += -D_WITHOUT_THREAD_EXCEPTIONS
}
CONFIG(release_NO_EXCEPTIONS, debug|release|debug_NO_EXCEPTIONS|release_NO_EXCEPTIONS) {
    DESTDIR = ../../ReleaseClang_NO_EXCEPTIONS
    QMAKE_CXXFLAGS += -D_WITHOUT_THREAD_EXCEPTIONS
}
CONFIG(debug, debug|release|debug_NO_EXCEPTIONS|release_NO_EXCEPTIONS) {
    DESTDIR = ../../DebugClang
}
CONFIG(release, debug|release|debug_NO_EXCEPTIONS|release_NO_EXCEPTIONS) {
    DESTDIR = ../../ReleaseClang
}
}

contains(QMAKE_CXX, g++) {
CONFIG(debug_NO_EXCEPTIONS, debug|release|debug_NO_EXCEPTIONS|release_NO_EXCEPTIONS) {
    DESTDIR = ../../Debug_NO_EXCEPTIONS
    QMAKE_CXXFLAGS += -D_WITHOUT_THREAD_EXCEPTIONS
}
CONFIG(release_NO_EXCEPTIONS, debug|release|debug_NO_EXCEPTIONS|release_NO_EXCEPTIONS) {
    DESTDIR = ../../Release_NO_EXCEPTIONS
    QMAKE_CXXFLAGS += -D_WITHOUT_THREAD_EXCEPTIONS
}
CONFIG(debug, debug|release|debug_NO_EXCEPTIONS|release_NO_EXCEPTIONS) {
    DESTDIR = ../../Debug
}
CONFIG(release, debug|release|debug_NO_EXCEPTIONS|release_NO_EXCEPTIONS) {
    DESTDIR = ../../Release
}
}

contains(QMAKE_CXX, icpc) {
QMAKE_RPATHDIR += /opt/intel/system_studio_2019/compilers_and_libraries_2019.5.281/linux/compiler/lib/intel64_lin
CONFIG(debug_NO_EXCEPTIONS, debug|release|debug_NO_EXCEPTIONS|release_NO_EXCEPTIONS) {
    DESTDIR = ../../DebugIntel_NO_EXCEPTIONS
    QMAKE_CXXFLAGS += -D_WITHOUT_THREAD_EXCEPTIONS
}
CONFIG(release_NO_EXCEPTIONS, debug|release|debug_NO_EXCEPTIONS|release_NO_EXCEPTIONS) {
    DESTDIR = ../../ReleaseIntel_NO_EXCEPTIONS
    QMAKE_CXXFLAGS += -D_WITHOUT_THREAD_EXCEPTIONS
}
CONFIG(debug, debug|release|debug_NO_EXCEPTIONS|release_NO_EXCEPTIONS) {
    DESTDIR = ../../DebugIntel
}
CONFIG(release, debug|release|debug_NO_EXCEPTIONS|release_NO_EXCEPTIONS) {
    DESTDIR = ../../ReleaseIntel
}
}

QMAKE_CXXFLAGS += -Wno-unused-variable -Wno-unused-parameter
INCLUDEPATH += ../PThreadClassLib
DEPENDPATH += $$INCLUDEPATH

#QMAKE_CXXFLAGS += -std=c++0x -I../../../PThreadClassLib
QMAKE_LFLAGS += -L$$DESTDIR
QMAKE_RPATHDIR += $$DESTDIR .
#QMAKE_RPATHDIR += .

QMAKE_CLEAN += $$DESTDIR/$$TARGET

SOURCES += \
    PThreadTest.cpp

LIBS += -L$$DESTDIR -lPThreadClassLib -lpthread
