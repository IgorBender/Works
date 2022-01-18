/*
 *  TestDefinitions.h
 *
 *  Created on: 2021-11-19
 *  Author: Igor Bender
 *
 */
#ifndef TESTDEFINITIONS_H
#define TESTDEFINITIONS_H

enum class TestSystemState
{
    StartSystemState,
    WorkingSystemState1,
    WorkingSystemState2,
    EndSystemState
};

enum class TetsStatetsTransition
{
    ToWorkingSystemState1,
    ToWorkingSystemState2,
    ToEndState
};

#endif // TESTDEFINITIONS_H
