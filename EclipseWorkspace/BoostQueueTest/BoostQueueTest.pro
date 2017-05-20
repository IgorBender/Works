TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -Wno-unused-parameter

CONF_VAR = $$(CONFIG_NAME)

contains(CONF_VAR, DebugConf) {
    DESTDIR = ../BoostQueueTest/Debug
}
contains(CONF_VAR, ReleaseConf) {
    DESTDIR = ../BoostQueueTest/Release
}

contains(CONF_VAR, DebugClang) {
    DESTDIR = ../BoostQueueTest/DebugClang
}
contains(CONF_VAR, ReleaseClang) {
    DESTDIR = ../BoostQueueTest/ReleaseClang
}

SOURCES += \
    BoostQueueTest.cpp

LIBS += -lpthread

QMAKE_CLEAN += $$DESTDIR/$$TARGET*

HEADERS +=
