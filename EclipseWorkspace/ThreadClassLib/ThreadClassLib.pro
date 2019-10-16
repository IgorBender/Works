#-------------------------------------------------
#
# Project created by QtCreator 2018-11-25T17:18:12
#
#-------------------------------------------------

QT       -= core gui

TARGET = ThreadClassLib
TEMPLATE = lib

DEFINES += THREADCLASSLIB_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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
        ThreadClassLib.cpp \
    CondVarClass.cpp \
    ThreadClass.cpp \
    ThreadException.cpp

HEADERS += \
        ThreadClassLib.h \
    CondVarClass.h \
    ThreadClass.h \
    ThreadException.h

QMAKE_CLEAN += $$libTARGET
