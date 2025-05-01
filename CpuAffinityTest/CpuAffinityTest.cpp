/**********************************************************************
 * File : CpuAffinityTest.cpp
 * Created by Igor Bender 2022-12-06
 * ********************************************************************/

/**********************************************************************
 * Explore the way to achive affinity of a thread to certain CPU and
 * assure no other task can be ran on the very same CPU.
 * It is recomended to instruct the kernel not to account on choosen CPU
 * in regular scheduling, the CPU will be taken by cpu_set to be base for
 * thread affinity, thus providing the thread will solely own the CPU.
 * The CPU dedication in kernel can be acheived by kernel commad line
 * parameter isolcpus= .
 * ********************************************************************/

#include "TimingTest.h"

int main()
{
    // For posiibility of change thread's priority.
    // --------------------------------------------
//    sched_param Param;
//    Param.__sched_priority = 10;
//    int SchedRes =  sched_setscheduler(0, SCHED_FIFO, &Param);
//    if(SchedRes)
//    {
//        cout << "Wrong sched_setscheduler" << endl;
//    }
    // --------------------------------------------
    TimingTest Test;
    Test.test();

    return 0;
}
