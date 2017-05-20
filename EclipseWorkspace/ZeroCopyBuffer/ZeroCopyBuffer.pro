TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -Wno-unused-parameter

CONF_VAR = $$(CONFIG_NAME)

contains(CONF_VAR, DebugConf) {
    DESTDIR = ../ZeroCopyBuffer/Debug
}
contains(CONF_VAR, ReleaseConf) {
    DESTDIR = ../ZeroCopyBuffer/Release
}

contains(CONF_VAR, DebugClang) {
    DESTDIR = ../ZeroCopyBuffer/DebugClang
}
contains(CONF_VAR, ReleaseClang) {
    DESTDIR = ../ZeroCopyBuffer/ReleaseClang
}

SOURCES += \
    ZeroCopyBuffer.cpp

HEADERS += \
    CharBuffersPool.h \
    NotificationQueue.h \
    ResorcesPool.h \
    ZeroCopyBuffer.h

LIBS += -lpthread

QMAKE_CLEAN += $$DESTDIR/$$TARGET*
