/*
 *  StateMachineTestClass.cpp
 *
 *  Created on: 2021-11-19
 *  Author: Igor Bender
 *
 */
#include "StateMachineTestClass.h"
#include "TestSystemStates.h"
#include <iostream>
#include <thread>

bool StateMachineTestClass::init()
{
    m_StateMachine.addState(TestSystemState::StartSystemState, new StartSystemState(TestSystemState::StartSystemState, this));
    m_StateMachine.addState(TestSystemState::WorkingSystemState1, new WorkingSystemState1(TestSystemState::WorkingSystemState1, this));
    m_StateMachine.addState(TestSystemState::WorkingSystemState2, new WorkingSystemState2(TestSystemState::WorkingSystemState2, this));
    m_StateMachine.addState(TestSystemState::EndSystemState, new EndSystemState(TestSystemState::EndSystemState, this));

    m_StateMachine.addTransition(TestSystemState::StartSystemState, TetsStatetsTransition::ToWorkingSystemState1,
                                 TestSystemState::WorkingSystemState1);
    m_StateMachine.addTransition(TestSystemState::WorkingSystemState1, TetsStatetsTransition::ToWorkingSystemState2,
                                 TestSystemState::WorkingSystemState2);
    m_StateMachine.addTransition(TestSystemState::WorkingSystemState2, TetsStatetsTransition::ToWorkingSystemState1,
                                 TestSystemState::WorkingSystemState1);
    m_StateMachine.addTransition(TestSystemState::WorkingSystemState2, TetsStatetsTransition::ToWorkingSystemState2,
                                 TestSystemState::WorkingSystemState2);
    m_StateMachine.addTransition(TestSystemState::WorkingSystemState1, TetsStatetsTransition::ToEndState,
                                 TestSystemState::EndSystemState);

    m_StateMachine.start();

    return true;
}

bool StateMachineTestClass::down()
{
    return true;
}

bool StateMachineTestClass::enterStartState()
{
    std::cout << "Enter StartSystemState." << std::endl;
    return true;
}

bool StateMachineTestClass::exitStartState()
{
    std::cout << "Exit StartSystemState." << std::endl;
    return true;
}

bool StateMachineTestClass::enterWorkingState1()
{
    std::cout << "Enter WorkingState1." << std::endl;
    return true;
}

bool StateMachineTestClass::exitWorkingState1()
{
    std::cout << "Exit WorkingState1." << std::endl;
    return true;
}

bool StateMachineTestClass::workingState1()
{
    std::cout << "In WorkingState1." << std::endl;
    return true;
}

bool StateMachineTestClass::enterWorkingState2()
{
    std::cout << "Enter WorkingState2." << std::endl;
    return true;
}

bool StateMachineTestClass::exitWorkingState2()
{
    std::cout << "Exit WorkingState2." << std::endl;
    return true;
}

bool StateMachineTestClass::workingState2()
{
    std::cout << "In WorkingState2." << std::endl;
    return true;
}

bool StateMachineTestClass::endState()
{
    std::cout << "Enter EndSystemState." << std::endl;
    m_Exit = true;
    return true;
}

void StateMachineTestClass::testControlRoutine()
{
    if(m_StateMachine.transit(TetsStatetsTransition::ToWorkingSystemState2))
    {
        std::cerr << "Successful un-allowed transition error" << std::endl;
        return;
    }
    if(!m_StateMachine.transit(TetsStatetsTransition::ToWorkingSystemState1))
    {
        std::cerr << "unsuccessful allowed transition error" << std::endl;
        return;
    }
    std::cout << "Forbidden state change was prevented, continue test..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds{1});
    if(!m_StateMachine.transit(TetsStatetsTransition::ToWorkingSystemState2))
    {
        std::cerr << "unsuccessful allowed transition error" << std::endl;
        return;
    }
    std::this_thread::sleep_for(std::chrono::seconds{1});
    if(!m_StateMachine.transit(TetsStatetsTransition::ToWorkingSystemState2))
    {
        std::cerr << "unsuccessful allowed transition error" << std::endl;
        return;
    }
    std::this_thread::sleep_for(std::chrono::seconds{1});
    if(!m_StateMachine.transit(TetsStatetsTransition::ToWorkingSystemState2))
    {
        std::cerr << "unsuccessful allowed transition error" << std::endl;
        return;
    }
    std::this_thread::sleep_for(std::chrono::seconds{1});
    if(!m_StateMachine.transit(TetsStatetsTransition::ToWorkingSystemState1))
    {
        std::cerr << "unsuccessful allowed transition error" << std::endl;
        return;
    }
    std::this_thread::sleep_for(std::chrono::seconds{1});
    if(!m_StateMachine.transit(TetsStatetsTransition::ToEndState))
    {
        std::cerr << "unsuccessful allowed transition error" << std::endl;
        return;
    }
    std::this_thread::sleep_for(std::chrono::seconds{1});
    if(m_Exit)
        std::cout << "Going exit..." << std::endl;
}
