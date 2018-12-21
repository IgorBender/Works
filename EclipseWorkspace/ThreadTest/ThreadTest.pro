TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

CONFIG(debug, debug|release) {
    DESTDIR = ../../Debug
    QMAKE_RPATHDIR += /home/igor/tests/EclipseWorkspace/Debug
}
CONFIG(release, debug|release) {
    DESTDIR = ../../Release
    QMAKE_RPATHDIR += /home/igor/tests/EclipseWorkspace/Release
}

INCLUDEPATH += ../../ThreadClassLib
DEPENDPATH += INCLUDEPATH
LIBS += -L$$DESTDIR -lThreadClassLib -lpthread


SOURCES += \
    ThreadTest.cpp

QMAKE_CLEAN += $$TARGET
