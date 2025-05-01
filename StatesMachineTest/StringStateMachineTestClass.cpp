/*
 *  StringStateMachineTestClass.cpp
 *
 *  Created on: 2022-01-18
 *  Author: Igor Bender
 *
 */
#include "StringStateMachineTestClass.h"
#include "StringTestSystemStates.h"
#include <iostream>
#include <thread>

bool StringStateMachineTestClass::init()
{
    m_StateMachine.addState("Start", new StringStartSystemState("Start", this));
    m_StateMachine.addState("Working1", new StringWorkingSystemState1("Working1", this));
    m_StateMachine.addState("Working2", new StringWorkingSystemState2("Working2", this));
    m_StateMachine.addState("End", new StringEndSystemState("End", this));

    m_StateMachine.addTransition("Start", "ToWorking1", "Working1");
    m_StateMachine.addTransition("Working1", "ToWorking2", "Working2");
    m_StateMachine.addTransition("Working2", "ToWorking1", "Working1");
    m_StateMachine.addTransition("Working2", "ToWorking2", "Working2");
    m_StateMachine.addTransition("Working1", "ToEnd", "End");

    m_StateMachine.start();

    return true;
}

bool StringStateMachineTestClass::down()
{
    return true;
}

bool StringStateMachineTestClass::enterStartState()
{
    std::cout << "Enter StartSystemState." << std::endl;
    return true;
}

bool StringStateMachineTestClass::exitStartState()
{
    std::cout << "Exit StartSystemState." << std::endl;
    return true;
}

bool StringStateMachineTestClass::enterWorkingState1()
{
    std::cout << "Enter WorkingState1." << std::endl;
    return true;
}

bool StringStateMachineTestClass::exitWorkingState1()
{
    std::cout << "Exit WorkingState1." << std::endl;
    return true;
}

bool StringStateMachineTestClass::workingState1()
{
    std::cout << "In WorkingState1." << std::endl;
    return true;
}

bool StringStateMachineTestClass::enterWorkingState2()
{
    std::cout << "Enter WorkingState2." << std::endl;
    return true;
}

bool StringStateMachineTestClass::exitWorkingState2()
{
    std::cout << "Exit WorkingState2." << std::endl;
    return true;
}

bool StringStateMachineTestClass::workingState2()
{
    std::cout << "In WorkingState2." << std::endl;
    return true;
}

bool StringStateMachineTestClass::endState()
{
    std::cout << "Enter EndSystemState." << std::endl;
    m_Exit = true;
    return true;
}

void StringStateMachineTestClass::testControlRoutine()
{
    if(m_StateMachine.transit("ToWorking2"))
    {
        std::cerr << "Successful un-allowed transition error" << std::endl;
        return;
    }
    if(!m_StateMachine.transit("ToWorking1"))
    {
        std::cerr << "unsuccessful allowed transition error" << std::endl;
        return;
    }
    std::cout << "Forbidden state change was prevented, continue test..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds{1});
    if(!m_StateMachine.transit("ToWorking2"))
    {
        std::cerr << "unsuccessful allowed transition error" << std::endl;
        return;
    }
    std::this_thread::sleep_for(std::chrono::seconds{1});
    if(!m_StateMachine.transit("ToWorking2"))
    {
        std::cerr << "unsuccessful allowed transition error" << std::endl;
        return;
    }
    std::this_thread::sleep_for(std::chrono::seconds{1});
    if(!m_StateMachine.transit("ToWorking2"))
    {
        std::cerr << "unsuccessful allowed transition error" << std::endl;
        return;
    }
    std::this_thread::sleep_for(std::chrono::seconds{1});
    if(!m_StateMachine.transit("ToWorking1"))
    {
        std::cerr << "unsuccessful allowed transition error" << std::endl;
        return;
    }
    std::this_thread::sleep_for(std::chrono::seconds{1});
    if(!m_StateMachine.transit("ToEnd"))
    {
        std::cerr << "unsuccessful allowed transition error" << std::endl;
        return;
    }
    std::this_thread::sleep_for(std::chrono::seconds{1});
    if(m_Exit)
        std::cout << "Going exit..." << std::endl;
}
