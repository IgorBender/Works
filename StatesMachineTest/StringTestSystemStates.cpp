/*
 *  StringTestSystemStates.cpp
 *
 *  Created on: 2022-01-18
 *  Author: Igor Bender
 *
 */

#include "StringTestSystemStates.h"
#include "StringStateMachineTestClass.h"

bool StringStartSystemState::enter()
{
    return m_pApp->enterStartState();
}

bool StringStartSystemState::exit()
{
    return m_pApp->exitStartState();
}

bool StringWorkingSystemState1::enter()
{
    return m_pApp->enterWorkingState1();
}

bool StringWorkingSystemState1::exit()
{
    return m_pApp->exitWorkingState1();
}

bool StringWorkingSystemState1::run()
{
    return m_pApp->workingState1();
}

bool StringWorkingSystemState2::enter()
{
    return m_pApp->enterWorkingState2();
}

bool StringWorkingSystemState2::exit()
{
    return m_pApp->exitWorkingState2();
}

bool StringWorkingSystemState2::run()
{
    return m_pApp->workingState2();
}

bool StringEndSystemState::enter()
{
    return m_pApp->endState();
}

bool StringEndSystemState::exit()
{
    return true;
}
