/*
 *  StateMachineTestClass.h
 *
 *  Created on: 2021-11-19
 *  Author: Igor Bender
 *
 */
#ifndef STATEMACHINETESTCLASS_H
#define STATEMACHINETESTCLASS_H

#include "StateMachine.h"
#include "TestDefinitions.h"

/*
 * Example of state machine based on enumeration types for states and transitions.
 */
class StateMachineTestClass
{
public:
    StateMachineTestClass() : m_StateMachine(TestSystemState::StartSystemState){}
    ~StateMachineTestClass() {}

    bool init();
    bool down();

    bool enterStartState();
    bool exitStartState();
    bool enterWorkingState1();
    bool exitWorkingState1();
    bool workingState1();
    bool enterWorkingState2();
    bool exitWorkingState2();
    bool workingState2();
    bool endState();

    void testControlRoutine();

protected:
    StateMachineThreadSafe<TestSystemState, TetsStatetsTransition> m_StateMachine;
    bool m_Exit = false;
};

#endif // STATEMACHINETESTCLASS_H
