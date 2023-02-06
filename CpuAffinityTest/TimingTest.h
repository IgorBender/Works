/*
 *  TimingTest.h
 *
 *  Created on: 2023-02-06
 *  Author: Igor Bender
 *
 */
#ifndef TIMINGTEST_H
#define TIMINGTEST_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <thread>
#include <memory>
#include <array>
#include <sys/mman.h>
#include <time.h>

// Use ptheard wrapping library.
#include <PThreadExtended.h>

///
/// \brief The TimingTest class
///
///
class TimingTest
{
public:
    ///
    /// \brief TimingTest - constructor, no parameters
    ///
    TimingTest()
    {
        m_LastTime.tv_nsec = 0;
        m_LastTime.tv_sec = 0;
    }
    ///
    /// Destructor
    ~TimingTest(){}

    ///
    /// \brief test - test method
    /// \return 0 - success
    ///
    uint32_t test();

protected:
    uint32_t m_Counter = 0; /// Counter of test cycles
    static const int32_t m_Limit = 5000; /// Number of computation cycles
    int32_t m_PrimesForLoad = 0;
    int32_t m_PrimesForTest = 0;
    bool m_Started = false;
    bool m_End = false;
    timespec m_Start;
    timespec m_Stop;
    timespec m_LastTime;
    static const uint32_t NUMBER_OF_LOAD_THREADS = 16;
    double* m_pDurations = nullptr;
    static const uint32_t TEST_COUNTER = 5000;
    uint32_t m_CurrentTest = 0;

    // Thread routine for load CPUs
    void* cpuLoadThreadRoutine();
    static void* staticCpuLoadThreadRoutine(void* p)
    {
        return (reinterpret_cast<TimingTest*>(p))->cpuLoadThreadRoutine();
    }
    // Thread routine for test
    void* cpuTestThreadRoutine();
    static void* staticCpuTestThreadRoutine(void* p)
    {
        return (reinterpret_cast<TimingTest*>(p))->cpuTestThreadRoutine();
    }
    void analyzeResults();
};

#endif // TIMINGTEST_H
