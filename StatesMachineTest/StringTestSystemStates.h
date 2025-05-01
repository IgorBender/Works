/*
 *  StringTestSystemStates.h
 *
 *  Created on: 2022-01-18
 *  Author: Igor Bender
 *
 */
#ifndef STRINGTESTSYSTEMSTATES_H
#define STRINGTESTSYSTEMSTATES_H

#include "SystemState.h"
#include <string>

class StringStateMachineTestClass;

class StringStartSystemState : public AppSystemState<std::string, StringStateMachineTestClass>
{
public:
    StringStartSystemState(std::string State, StringStateMachineTestClass* p) :
        AppSystemState<std::string, StringStateMachineTestClass>(State, p) {}
    virtual bool enter();
    virtual bool exit();
};

class StringWorkingSystemState1 : public AppSystemState<std::string, StringStateMachineTestClass>
{
public:
    StringWorkingSystemState1(std::string State, StringStateMachineTestClass* p) :
        AppSystemState<std::string, StringStateMachineTestClass>(State, p) {}
    virtual bool enter();
    virtual bool exit();
    virtual bool run();
};

class StringWorkingSystemState2 : public AppSystemState<std::string, StringStateMachineTestClass>
{
public:
    StringWorkingSystemState2(std::string State, StringStateMachineTestClass* p) :
        AppSystemState<std::string, StringStateMachineTestClass>(State, p) {}
    virtual bool enter();
    virtual bool exit();
    virtual bool run();
};

class StringEndSystemState : public AppSystemState<std::string, StringStateMachineTestClass>
{
public:
    StringEndSystemState(std::string State, StringStateMachineTestClass* p) :
        AppSystemState<std::string, StringStateMachineTestClass>(State, p) {}
    virtual bool enter();
    virtual bool exit();
};

#endif // STRINGTESTSYSTEMSTATES_H
