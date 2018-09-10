TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

CONFIG(debug, debug|release) {
    DESTDIR = ../../Debug
}
CONFIG(release, debug|release) {
    DESTDIR = ../../Release
}

#QMAKE_LFLAGS += -L$$DESTDIR
QMAKE_RPATHDIR += $$DESTDIR
INCLUDEPATH += ../PThreadClassLib
DEPENDPATH += $$INCLUDEPATH

SOURCES += \
    PThreadTest.cpp

LIBS += -L$$DESTDIR -lPThreadClassLib -lpthread
QMAKE_CLEAN += $$DESTDIR/$$TARGET*
