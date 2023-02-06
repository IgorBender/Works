/*
 *  TimingTest.cpp
 *
 *  Created on: 2023-02-06
 *  Author: Igor Bender
 *
 */
#include "TimingTest.h"

// Auxiliary average computation class
#include "AccumulatedAverage.h"

using namespace std;

// Auxilliary time duration functions
constexpr double diff_timespec_sec(const struct timespec *time1,
                                   const struct timespec *time0)
{
  return (time1->tv_sec - time0->tv_sec)
      + (time1->tv_nsec - time0->tv_nsec) / 1000000000.0;
}

constexpr double diff_timespec_m(const struct timespec *time1,
                                 const struct timespec *time0)
{
  return (time1->tv_sec - time0->tv_sec) * 1000.0
      + (time1->tv_nsec - time0->tv_nsec) / 1000000.0;
}

constexpr double diff_timespec_u(const struct timespec *time1,
                                 const struct timespec *time0)
{
  return (time1->tv_sec - time0->tv_sec) * 1000000.0
      + (time1->tv_nsec - time0->tv_nsec) / 1000.0;
}
// ----------------------------------

void* TimingTest::cpuLoadThreadRoutine()
{
    for(int32_t num = 1; num <= m_Limit; ++num)
    {
        int32_t i = 2;
        while(i <= num)
        {
            if(num % i == 0)
                break;
            i++;
        }
        if(i == num)
           ++m_PrimesForLoad;
    }
    return nullptr;
}

void* TimingTest::cpuTestThreadRoutine()
{
    if(!m_Started)
    {
        m_Started = true;
        clock_gettime(CLOCK_REALTIME, &m_Start);
    }

    for(int32_t num = 1; num <= m_Limit; ++num)
    {
        int32_t i = 2;
        while(i <= num)
        {
            if(num % i == 0)
                break;
            i++;
        }
        if(i == num)
            ++m_PrimesForTest;
    }

    timespec CurrentTime;
    clock_gettime(CLOCK_REALTIME, &CurrentTime);
    if(0 != m_LastTime.tv_sec && m_CurrentTest < TEST_COUNTER)
    {
        m_pDurations[m_CurrentTest] =
                diff_timespec_u(&CurrentTime, &m_LastTime);
        ++m_CurrentTest;
    }
    m_LastTime = CurrentTime;

    if(m_End)
        clock_gettime(CLOCK_REALTIME, &m_Stop);

    ++m_Counter;
    return nullptr;
}

uint32_t TimingTest::test()
{
    // Allocate memory for storage of tests timings.
    m_pDurations = new double[TEST_COUNTER];

    // Lock allocated memory
    int32_t LockStatus = mlockall(MCL_CURRENT);
    if(0 != LockStatus)
        cerr << "Memory lock failed" << endl;

    // Thread pool for simulation CPUs load, number of threads should be equal
    // or more than number of CPUs.
    array<shared_ptr<
            PThreadExtended>, TimingTest::NUMBER_OF_LOAD_THREADS> LoadThreads;

    THREAD_TRY
    {
        // Create threads as cyclic with no timeout.
        for(auto& p : LoadThreads)
        {
            shared_ptr<PThreadExtended> pTmp(
                        new PThreadExtended(
                            Runnable(
                                PThreadRoutineType(
                                    TimingTest::staticCpuLoadThreadRoutine),
                                nullptr, this),
                            PTHREAD_INFINITE, true));
            p = pTmp;
            p->run();
        }

        // Construct test thread as cyclic with no timeout.
        PThreadExtended Thread(
                    Runnable(
                        PThreadRoutineType(
                            TimingTest::staticCpuTestThreadRoutine),
                        nullptr, this),
                    PTHREAD_INFINITE, true);
        // Comment out this if test thread priority should not to be changed.
        // ------------------------------------------------------------------
//        Thread.setPolicy(SCHED_FIFO);
//        Thread.setPriority(98);
        // ------------------------------------------------------------------
        Thread.run();

        // Comment out this if test thread affinity should not be done.
        // ------------------------------------------------------------
        cpu_set_t Cpu;
        CPU_ZERO(&Cpu);
        // Get default thread CPU affinity, should be all available
        // processors (not isolated).
        int32_t Res = pthread_getaffinity_np(Thread.getThreadId(),
                                             sizeof(cpu_set_t), &Cpu);
        if(0 != Res)
        {
            THREAD_EXCEPT_THROW("Get affinity");
        }
        cout << " == Thread can run  on " << CPU_COUNT(&Cpu) << "  CPU"
             << (1 == CPU_COUNT(&Cpu) ? "" : "s") << " ==" << endl;

        cout << " -- Set cpu_set for thread --" << endl;
        CPU_ZERO(&Cpu);
        CPU_SET(1, &Cpu);
        cout << " == Thread can run  on " << CPU_COUNT(&Cpu) << "  CPU"
             << (1 == CPU_COUNT(&Cpu) ? "" : "s") << " ==" << endl;
        Res = pthread_setaffinity_np(Thread.getThreadId(),
                                     sizeof(cpu_set_t), &Cpu);
        if(0 != Res)
        {
            THREAD_EXCEPT_THROW("Set affinity");
        }
        // ------------------------------------------------------------

        for(auto& p : LoadThreads)
            p->start();

        Thread.start();

        while(TEST_COUNTER > m_Counter)
        {
            this_thread::sleep_for(chrono::seconds{1});
        }
        m_End = true;
        this_thread::sleep_for(chrono::seconds{1});
        Thread.stop();
        Thread.exit();
        Thread.join();

        for(auto& p : LoadThreads)
        {
            p->stop();
            p->exit();
            p->join();
        }

        cout << fixed
             << diff_timespec_sec(&m_Stop, &m_Start) << " seconds" << endl;
    }
    THREAD_EXCEPT_CATCH_BEGIN_NOREP
        if(m_pDurations)
        {
            munlockall();
            delete [] m_pDurations;
            m_pDurations = 0;
        }
        return 1;
    THREAD_EXCEPT_CATCH_END

    analyzeResults();

    if(m_pDurations)
    {
        munlockall();
        delete [] m_pDurations;
        m_pDurations = 0;
    }
    return 0;
}

void TimingTest::analyzeResults()
{
    // Open file for store cycle durations, file name is Results.txt and
    // it's located in working directory aside executable. To sorted view
    // run in shell command line :
    // sort Results.txt
    // or for saving sorted data :
    // sort Results.txt | cat > sorted.txt
    ofstream Results;
    Results.open("Results.txt");

    AccumulatedAverage<double> Duration;
    double MaxTime = numeric_limits<double>::min();
    double MinTime = numeric_limits<double>::max();
    for(uint32_t i = 0; i < TEST_COUNTER; ++i)
    {
        MaxTime = max(m_pDurations[i], MaxTime);
        MinTime = min(m_pDurations[i], MinTime);
        Duration.accumulate(m_pDurations[i]);
        if(!Results)
            continue;
        Results << m_pDurations[i] << endl;
    }
    Results.close();

    uint32_t NumberOfThreshholdDeviations = 0;
    const double DEVIATION_TRESHOLD = 1000.0;

    for(uint32_t i = 0; i < TEST_COUNTER; ++i)
    {
        if(DEVIATION_TRESHOLD < abs(m_pDurations[i]) - Duration.getAverage())
            ++NumberOfThreshholdDeviations;
    }

    cout << "Average cycle time " << Duration.getAverage()
         << " microseconds" << endl;
    cout << "Minimal cycle time " << MinTime << " microseconds, deviation "
         << MinTime - Duration.getAverage() << " microseconds" << endl;
    cout << "Maximal cycle time " << MaxTime << " microseconds, deviation "
         << MaxTime - Duration.getAverage() << " microseconds" << endl;
    cout << "Number of deviations above " << DEVIATION_TRESHOLD
         << " microseconds - " << NumberOfThreshholdDeviations << endl;
}

