TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -Wno-unused-parameter

CONFIG(debug, debug|release) {
    DESTDIR = ../Debug
}
CONFIG(release, debug|release) {
    DESTDIR = ../Release
}

FLEXSOURCES = DefTests.l Parser.l
BISONSOURCES = DefTests.y Parser.y

SOURCES += \
    ParserTest.cpp \
    ParserInterface.cpp

QMAKE_CLEAN += $$DESTDIR/$$TARGET*

HEADERS += \
    ParserInterface.h

flex.commands = flex ${QMAKE_FILE_IN}
flex.input = FLEXSOURCES
flex.output = ${QMAKE_FILE_BASE}Lex.cpp
flex.variable_out = SOURCES
#flex.depends = FLEXSOURCES.h
flex.name = Flex Sources  ${QMAKE_FILE_IN}
flex.CONFIG += target_predeps
QMAKE_EXTRA_COMPILERS += flex

bison.commands = bison ${QMAKE_FILE_IN}
bison.input = BISONSOURCES
bison.output = ${QMAKE_FILE_BASE}.cpp
bison.variable_out = SOURCES
bison.name = Bison Sources  ${QMAKE_FILE_IN}
bison.CONFIG += target_predeps
QMAKE_EXTRA_COMPILERS += bison

OTHER_FILES += \
    $$BISONSOURCES \
    $$FLEXSOURCES
