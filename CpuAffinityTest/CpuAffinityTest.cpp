/**********************************************************************
 * File : CpuAffinityTest.cpp
 * Created by Igor Bender 2022-12-06
 * ********************************************************************/

/**********************************************************************
 * Explore the way to achive affinity of a thread to certain CPU and
 * assure no other task can be ran on the very same CPU.
 * ********************************************************************/

#include <iostream>
#include <iomanip>
#include <thread>
#include <cmath>

// Use ptheard wrapping library.
#include <PThreadExtended.h>

using namespace std;

static double Dummy = 0.0;

// Thread routine
void cpuLoadThreadRoutine()
{
    for(uint32_t i = 0; i < 10000000; ++i)
        Dummy = 81.0, Dummy = sqrt(Dummy) * 184.78;
}


int main()
{
    THREAD_TRY
    {
        // Construct thread
        PThreadExtended Thread(Runnable(PThreadRoutineType(cpuLoadThreadRoutine)), PTHREAD_INFINITE, true);
        Thread.run();

        cpu_set_t Cpu;
        CPU_ZERO(&Cpu);
        int32_t Res = pthread_getaffinity_np(Thread.getThreadId(), sizeof(cpu_set_t), &Cpu);
        if(0 != Res)
        {
            THREAD_EXCEPT_THROW("Get affinity");
        }
        cout << " == Thread can run  on " << CPU_COUNT(&Cpu) << "  CPU" << (1 == CPU_COUNT(&Cpu) ? "" : "s") << " ==" << endl;

        cout << " -- Set cpu_set for thread --" << endl;
        CPU_ZERO(&Cpu);
        CPU_SET(1, &Cpu);
        cout << " == Thread can run  on " << CPU_COUNT(&Cpu) << "  CPU" << (1 == CPU_COUNT(&Cpu) ? "" : "s") << " ==" << endl;
        Res = pthread_setaffinity_np(Thread.getThreadId(), sizeof(cpu_set_t), &Cpu);
        if(0 != Res)
        {
            THREAD_EXCEPT_THROW("Set affinity");
        }
        Thread.start();
        this_thread::sleep_for(chrono::seconds{5});
        Thread.stop();
        Thread.exit();
    }
    THREAD_EXCEPT_CATCH_BEGIN_NOREP
        return 1;
    THREAD_EXCEPT_CATCH_END


    return 0;
}
