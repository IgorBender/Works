/*
 *  TestSystemStates.h
 *
 *  Created on: 2021-11-19
 *  Author: Igor Bender
 *
 */
#ifndef TESTSYSTEMSTATES_H
#define TESTSYSTEMSTATES_H

#include "SystemState.h"
#include "TestDefinitions.h"

class StateMachineTestClass;

class StartSystemState : public AppSystemState<TestSystemState, StateMachineTestClass>
{
public:
    StartSystemState(TestSystemState State, StateMachineTestClass* p) :
        AppSystemState<TestSystemState, StateMachineTestClass>(State, p) {}
    virtual bool enter();
    virtual bool exit();
};

class WorkingSystemState1 : public AppSystemState<TestSystemState, StateMachineTestClass>
{
public:
    WorkingSystemState1(TestSystemState State, StateMachineTestClass* p) :
        AppSystemState<TestSystemState, StateMachineTestClass>(State, p) {}
    virtual bool enter();
    virtual bool exit();
    virtual bool run();
};

class WorkingSystemState2 : public AppSystemState<TestSystemState, StateMachineTestClass>
{
public:
    WorkingSystemState2(TestSystemState State, StateMachineTestClass* p) :
        AppSystemState<TestSystemState, StateMachineTestClass>(State, p) {}
    virtual bool enter();
    virtual bool exit();
    virtual bool run();
};

class EndSystemState : public AppSystemState<TestSystemState, StateMachineTestClass>
{
public:
    EndSystemState(TestSystemState State, StateMachineTestClass* p) :
        AppSystemState<TestSystemState, StateMachineTestClass>(State, p) {}
    virtual bool enter();
    virtual bool exit();
};

#endif // TESTSYSTEMSTATES_H
