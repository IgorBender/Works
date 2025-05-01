/*
 *  TestSystemStates.cpp
 *
 *  Created on: 2021-11-19
 *  Author: Igor Bender
 *
 */

#include "TestSystemStates.h"
#include "StateMachineTestClass.h"


bool StartSystemState::enter()
{
    return m_pApp->enterStartState();
}

bool StartSystemState::exit()
{
    return m_pApp->exitStartState();
}

bool WorkingSystemState1::enter()
{
    return m_pApp->enterWorkingState1();
}

bool WorkingSystemState1::exit()
{
    return m_pApp->exitWorkingState1();
}

bool WorkingSystemState1::run()
{
    return m_pApp->workingState1();
}

bool WorkingSystemState2::enter()
{
    return m_pApp->enterWorkingState2();
}

bool WorkingSystemState2::exit()
{
    return m_pApp->exitWorkingState2();
}

bool WorkingSystemState2::run()
{
    return m_pApp->workingState2();
}

bool EndSystemState::enter()
{
    return m_pApp->endState();
}

bool EndSystemState::exit()
{
    return true;
}


