#-------------------------------------------------
#
# Project created by QtCreator 2016-10-18T03:35:42
#
#-------------------------------------------------

QT       -= core gui

TARGET = PThreadClassLib
TEMPLATE = lib

DEFINES += PTHREADCLASSLIB_LIBRARY
CONFIG += c++11

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

INCLUDEPATH += .

VERSION = 2.3.3

QMAKE_CLEAN += $$DESTDIR/lib$$TARGET*

SOURCES += \
    CondVarClass.cpp \
    PBarrierClass.cpp \
    PMutexClass.cpp \
    PRWLockClass.cpp \
    PThreadClass.cpp \
    PThreadClassLib.cpp \
    PThreadExtended.cpp \
    ThreadException.cpp

HEADERS +=\
    CondVarClass.h \
    PBarrierClass.h \
    PMutexClass.h \
    PRWLockClass.h \
    PThreadBaseException.h \
    PThreadClass.h \
    PThreadClassLib.h \
    PThreadExtended.h \
    ThreadException.h


