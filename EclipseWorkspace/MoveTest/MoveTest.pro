TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

CONF_VAR = $$(CONFIG_NAME)

contains(CONF_VAR, DebugConf) {
    DESTDIR = ../MoveTest/Debug
}
contains(CONF_VAR, ReleaseConf) {
    DESTDIR = ../MoveTest/Release
}

contains(CONF_VAR, DebugClang) {
    DESTDIR = ../MoveTest/DebugClang
}
contains(CONF_VAR, ReleaseClang) {
    DESTDIR = ../MoveTest/ReleaseClang
}

SOURCES += \
    MoveTest.cpp

QMAKE_CLEAN += $$DESTDIR/$$TARGET*
