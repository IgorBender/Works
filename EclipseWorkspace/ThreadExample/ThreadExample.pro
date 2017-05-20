TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -Wno-unused-parameter

CONF_VAR = $$(CONFIG_NAME)

contains(CONF_VAR, DebugConf) {
    DESTDIR = ../ThreadExample/Debug
}
contains(CONF_VAR, ReleaseConf) {
    DESTDIR = ../ThreadExample/Release
}

contains(CONF_VAR, DebugClang) {
    DESTDIR = ../ThreadExample/DebugClang
}
contains(CONF_VAR, ReleaseClang) {
    DESTDIR = ../ThreadExample/ReleaseClang
}

SOURCES += \
    ThreadExample.cpp

HEADERS += \
    ThreadExample.h

LIBS += -lpthread

QMAKE_CLEAN += $$DESTDIR/$$TARGET*
