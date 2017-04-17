TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

CONF_VAR = $$(CONFIG_NAME)

contains(CONF_VAR, DebugConf) {
    DESTDIR = ../PugiXmlTest/Debug
}
contains(CONF_VAR, ReleaseConf) {
    DESTDIR = ../PugiXmlTest/Release
}

contains(CONF_VAR, DebugClang) {
    DESTDIR = ../PugiXmlTest/DebugClang
}
contains(CONF_VAR, ReleaseClang) {
    DESTDIR = ../PugiXmlTest/ReleaseClang
}


SOURCES += \
    PugiXmlTest.cpp

LIBS += -lpugixml
QMAKE_CLEAN += $$DESTDIR/$$TARGET*
