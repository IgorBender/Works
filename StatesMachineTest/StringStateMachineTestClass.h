/*
 *  StringStateMachineTestClass.h
 *
 *  Created on: 2022-01-18
 *  Author: Igor Bender
 *
 */
#ifndef STRINGSTATEMACHINETESTCLASS_H
#define STRINGSTATEMACHINETESTCLASS_H

#include "StateMachine.h"
#include <string>

class StringStateMachineTestClass
{
public:
    StringStateMachineTestClass(): m_StateMachine("Start") {};
    ~StringStateMachineTestClass() {}

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
    StateMachineThreadSafe<std::string, std::string> m_StateMachine;
    bool m_Exit = false;
};

#endif // STRINGSTATEMACHINETESTCLASS_H
