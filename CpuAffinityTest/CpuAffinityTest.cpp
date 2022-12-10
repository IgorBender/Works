/**********************************************************************
 * File : CpuAffinityTest.cpp
 * Created by Igor Bender 2022-12-06
 * ********************************************************************/

/**********************************************************************
 * Explore the way to achive affinity of a thread to certain CPU and
 * assure no other task can be ran on the very same CPU.
 * It is recomended to instruct the kernel not to account on chusen CPU
 * in regular shceduling, the CPU will be taken by cpu_set to be base for
 * thread affinity, thus providing the thread will own the CPU solely.
 * The CPU dedication in kernel can be acheived by kernel commad line
 * parameter isolcpus= .
 * ********************************************************************/

#include <iostream>
#include <iomanip>
#include <thread>

// Use ptheard wrapping library.
#include <PThreadExtended.h>

using namespace std;


static uint32_t Counter = 0;

int32_t limit = 5000;
int32_t primes = 0;
bool Started = false;
bool End = false;
chrono::high_resolution_clock::time_point Start;
chrono::high_resolution_clock::time_point Stop;
const uint32_t NUMBER_OF_LOAD_THREADS = 16;

// Thread routine for load CPUs
void cpuLoadThreadRoutine()
{
    for(int32_t num = 1; num <= limit; ++num)
    {
         int32_t i = 2;
         while(i <= num)
         {
             if(num % i == 0)
                 break;
             i++;
         }
         if(i == num)
             primes++;
    }
}

// Thread routine for test
void cpuTestThreadRoutine()
{
    if(!Started)
    {
        Started = true;
        Start = chrono::high_resolution_clock::now();
    }

    for(int32_t num = 1; num <= limit; ++num)
    {
         int32_t i = 2;
         while(i <= num)
         {
             if(num % i == 0)
                 break;
             i++;
         }
         if(i == num)
             primes++;
    }

    if(End)
        Stop = chrono::high_resolution_clock::now();


    ++Counter;
}


int main()
{
    // Thread pool for simulation CPUs load, number of threads is equal or more
    // than number of CPUs.
    PThreadExtended* LoadThreads[NUMBER_OF_LOAD_THREADS];

    THREAD_TRY
    {
        // Craete threads as cyclic with no timeout.
        for(uint32_t i = 0; i < NUMBER_OF_LOAD_THREADS; ++i)
        {
            LoadThreads[i] = new PThreadExtended(
                        Runnable(PThreadRoutineType(cpuLoadThreadRoutine)),
                        PTHREAD_INFINITE, true);
            LoadThreads[i]->run();
        }
        // Construct test thread as cyclic with no timeout.
        PThreadExtended Thread(
                    Runnable(PThreadRoutineType(cpuTestThreadRoutine)),
                    PTHREAD_INFINITE, true);
        Thread.run();

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
        for(uint32_t i = 0; i < NUMBER_OF_LOAD_THREADS; ++i)
        {
            LoadThreads[i]->start();
        }
        Thread.start();
        while(5000 > Counter)
        {
            this_thread::sleep_for(chrono::seconds{1});
        }
        End = true;
        this_thread::sleep_for(chrono::seconds{1});
        Thread.stop();
        Thread.exit();
        for(uint32_t i = 0; i < NUMBER_OF_LOAD_THREADS; ++i)
        {
            LoadThreads[i]->stop();
            LoadThreads[i]->exit();
        }

        cout << fixed
             << chrono::duration<double, ratio<1,1> >(Stop - Start).count()
             << " seconds" << endl;
    }
    THREAD_EXCEPT_CATCH_BEGIN_NOREP
        return 1;
    THREAD_EXCEPT_CATCH_END

    return 0;
}