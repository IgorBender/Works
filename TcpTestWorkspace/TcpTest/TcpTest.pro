TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++0x
QMAKE_LFLAGS +=

CONF_VAR = $$(CONFIG_NAME)

contains(CONF_VAR, DebugConf) {
    DESTDIR = ../Debug
}
contains(CONF_VAR, ReleaseConf) {
    DESTDIR = ../Release
}

QMAKE_LFLAGS += -L$$DESTDIR
QMAKE_RPATHDIR += $$DESTDIR


SOURCES += \
    TcpTest.cpp \
    ../common/TcpConnection.cpp \
    ../common/TestTcpConnection.cpp

HEADERS += \
    ../common/CharBuffersPool.h \
    ../common/InterfaceDefs.h \
    ../common/ResorcesPool.h \
    ../common/TcpConnection.h \
    ../common/TestTcpConnection.h

DEPENDPATH += ../common ../sockclasslib ../ThreadClassLib
INCLUDEPATH += ../common ../sockclasslib ../ThreadClassLib
LIBS += -lsockclasslib -lThreadClassLib -lpthread
