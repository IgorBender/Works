/*
 *  StatesMachineTest.cpp
 *
 *  Created on: 2021-11-18
 *  Author: Igor Bender
 *
 */

#include "StateMachineTestClass.h"
#include "StringStateMachineTestClass.h"


int main()
{
    StateMachineTestClass Test;
    Test.init();
    Test.testControlRoutine();
    Test.down();

    StringStateMachineTestClass Test2;
    Test2.init();
    Test2.testControlRoutine();
    Test2.down();

    return 0;
}
