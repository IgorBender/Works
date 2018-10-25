#-------------------------------------------------
#
# Project created by QtCreator 2016-10-18T03:35:42
#
#-------------------------------------------------

QT       -= core gui

TARGET = PThreadClassLib
TEMPLATE = lib

DEFINES += PTHREADCLASSLIB_LIBRARY

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

QMAKE_CXXFLAGS += -std=c++0x -I.

VERSION = 2.3.2

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


