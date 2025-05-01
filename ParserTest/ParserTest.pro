TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -Wno-unused-parameter

contains(QMAKE_CXX, clang++) {
CONFIG(debug, debug|release) {
    DESTDIR = ../../DebugClang
}
CONFIG(release, debug|release) {
    DESTDIR = ../../ReleaseClang
}
}

contains(QMAKE_CXX, g++) {
CONFIG(debug, debug|release) {
    DESTDIR = ../../Debug
}
CONFIG(release, debug|release) {
    DESTDIR = ../../Release
}
}

contains(QMAKE_CXX, icpc) {
QMAKE_RPATHDIR += /opt/intel/sw_dev_tools/lib/intel64
CONFIG(debug, debug|release) {
    DESTDIR = ../../DebugIntel
}
CONFIG(release, debug|release) {
    DESTDIR = ../../ReleaseIntel
}
}

FLEXSOURCES = DefTests.l Parser.l
BISONSOURCES = DefTests.y Parser.y

SOURCES += \
    BitParserInterface.cpp \
    ParserTest.cpp

QMAKE_CLEAN += $$DESTDIR/$$TARGET*

HEADERS += \
    BitParserInterface.h

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
