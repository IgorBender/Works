TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

CONF_VAR = $$(CONFIG_NAME)

contains(CONF_VAR, DebugConf) {
    DESTDIR = ../CryptTest/Debug
}
contains(CONF_VAR, ReleaseConf) {
    DESTDIR = ../CryptTest/Release
}

contains(CONF_VAR, DebugClang) {
    DESTDIR = ../CryptTest/DebugClang
}
contains(CONF_VAR, ReleaseClang) {
    DESTDIR = ../CryptTest/ReleaseClang
}

SOURCES += \
    CryptTest.cpp

QMAKE_CLEAN += $$DESTDIR/$$TARGET*
