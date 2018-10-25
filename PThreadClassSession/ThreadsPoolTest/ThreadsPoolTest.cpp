/* 
 * File:   ThreadsPoolTest.cpp
 * Author: igor
 *
 * Created on November 2, 2009, 8:36 PM
 */

#include <stdlib.h>
#include <TrackingThreadsPool.h>

#ifdef __SunOS
#include <priv.h>
#else
#define _USE_MATH_DEFINES 1
#include <math.h>
#endif

#ifdef __linux__
#include <time.h>
#endif

void*
job0(void* p)
{
    //    cout << "--- Start job " << *(reinterpret_cast<int*>(p)) << endl;
#ifdef __SunOS
    hrtime_t StartTime = gethrtime();
#elif __linux__
    timespec StartTime;
    clock_gettime(CLOCK_MONOTONIC, &StartTime);
#else
    clock_t StartTime = clock();
#endif
    for(int j = 0; j < 200 - *(reinterpret_cast<int*>(p)); ++j)
        for(int i = 0; i < 400000; ++i)
        {
            double m = i * M_PI / 85.15 + sqrt(double(i^5));
        }
#ifdef __SunOS
    hrtime_t EndTime = gethrtime();
    //    cout << "--- End job " << *(reinterpret_cast<int*>(p)) << endl;
    cout << "Wall time of job " << *(reinterpret_cast<int*>(p)) << " " << fixed << double(EndTime - StartTime) / 1000.0 / 1000.0 << " miliseconds." << endl;
#elif __linux__
    timespec EndTime;
    clock_gettime(CLOCK_MONOTONIC, &EndTime);
    cout << "Wall time of job " << *(reinterpret_cast<int*>(p)) << " " << fixed << double(EndTime.tv_sec - StartTime.tv_sec) * 1000.0 +
        double(EndTime.tv_nsec - StartTime.tv_nsec) / 1000.0 / 1000.0 << " miliseconds." << endl;
#else
    clock_t EndTime = clock();
    cout << "Wall time of job " << *(reinterpret_cast<int*>(p)) << " " << fixed << double(EndTime - StartTime) / CLOCKS_PER_SEC * 1000.0 << " miliseconds." << endl;
#endif
    return NULL;
}

void*
job1(void* p)
{
    //    cout << "--- Start job " << *(reinterpret_cast<int*>(p)) << endl;
#ifdef __SunOS
    hrtime_t StartTime = gethrtime();
#elif __linux__
    timespec StartTime;
    clock_gettime(CLOCK_MONOTONIC, &StartTime);
#else
    clock_t StartTime = clock();
#endif
    for(int j = 0; j < 13 - *(reinterpret_cast<int*>(p)); ++j)
        for(int i = 0; i < 4000000; ++i)
        {
            double m = i * M_PI / 85.15 + sqrt(double(i^5));
        }
#ifdef __SunOS
    hrtime_t EndTime = gethrtime();
    //cout << "--- End job " << *(reinterpret_cast<int*>(p)) << endl;
    cout << "Wall time of job " << *(reinterpret_cast<int*>(p)) << " " << fixed << double(EndTime - StartTime) / 1000.0 / 1000.0 << " miliseconds." << endl;
#elif __linux__
    timespec EndTime;
    clock_gettime(CLOCK_MONOTONIC, &EndTime);
    cout << "Wall time of job " << *(reinterpret_cast<int*>(p)) << " " << fixed << double(EndTime.tv_sec - StartTime.tv_sec) * 1000.0 +
        double(EndTime.tv_nsec - StartTime.tv_nsec) / 1000.0 / 1000.0 << " miliseconds." << endl;
#else
    clock_t EndTime = clock();
    cout << "Wall time of job " << *(reinterpret_cast<int*>(p)) << " " << fixed << double(EndTime - StartTime) / CLOCKS_PER_SEC * 1000.0 << " miliseconds." << endl;
#endif
    return NULL;
}

void*
jobToBeCanceled(void* p)
{
    cout << "--- Start job " << *(reinterpret_cast<int*>(p)) << endl;
#ifndef WIN32
    //  hrtime_t StartTime = gethrtime();
#else
    clock_t StartTime = clock();
#endif
    for(int j = 0; j < 2000; ++j)
        for(int i = 0; i < 4000000; ++i)
        {
            double m = i * M_PI / 85.15 + sqrt(double(i^5));
        }
#ifndef WIN32
    //    hrtime_t EndTime = gethrtime();
    cout << "--- End job " << *(reinterpret_cast<int*>(p)) << endl;
    //cout << "Wall time of job " << *(reinterpret_cast<int*>(p)) << " " << fixed << double(EndTime - StartTime) / 1000.0 / 1000.0 << " miliseconds." << endl;
#else
    clock_t EndTime = clock();
    cout << "Wall time of job " << *(reinterpret_cast<int*>(p)) << " " << fixed << double(EndTime - StartTime) / CLOCKS_PER_SEC * 1000.0 << " miliseconds." << endl;
#endif
    return NULL;
    }

void
cleanUp(void* p)
{
    cout << "CleanUp of job " << *(reinterpret_cast<int*>(p)) << flush << endl;
}

/*
 * 
 */
int main(int argc, char** argv)
{
    if(1 < argc)
    {
        cout << argv[0] << " has no command line arguments." << endl;
    }
#ifdef __linux__
    sched_param Param;
    Param.__sched_priority = 90;
    int SchedRes = sched_setscheduler(0, SCHED_FIFO, &Param);
    if(SchedRes)
    {
        cout << "Wrong sched_setscheduler" << endl;
    }
#endif
    //    cout << "Enter any symbol to start --> ";
    //    char c;
    //    cin >> c;

    //     sched_param Params;
    //     int TmpPolicy;
    //     if(pthread_getschedparam(pthread_self(), &TmpPolicy, &Params) != 0)
    //     {
    //         return false;
    //     }
    //     Params.sched_priority = 9;
    //     if(pthread_setschedparam(pthread_self(), TmpPolicy, &Params) != 0)
    //     {
    //         return false;
    //     }


    int Job1Id = 1;
    int Job2Id = 2;
    int Job3Id = 3;
    int Job4Id = 4;
    int Job5Id = 5;
    int Job6Id = 6;
    int Job7Id = 7;
    int Job8Id = 8;
    int Job9Id = 9;
    int Job10Id = 10;

    int Processors[8];
    for(int i = 0; i < 6; ++i)
    {
        Processors[i] = i + 2;
      }
#ifndef WIN32
    TrackingThreadsPool Pool(16, 92, 100, 700, 6000, Processors, 6);
#else
    TrackingThreadsPool Pool(16, THREAD_PRIORITY_NORMAL, 100, 700, 6000, Processors, 6);
#endif
    Pool.doJob(PThreadRoutineType(jobToBeCanceled), &Job1Id, PThreadCleanUpRoutineType(cleanUp), &Job1Id);

#ifndef WIN32
    sleep(7);
#else
    Sleep(7000);
#endif

    Pool.doJob(PThreadRoutineType(job0), &Job1Id, PThreadCleanUpRoutineType(cleanUp), &Job1Id); // 1 - long job
    Pool.doJob(PThreadRoutineType(job0), &Job2Id); // 2 - long job
    Pool.doJob(PThreadRoutineType(job0), &Job3Id); // 3 - long job
    Pool.doJob(PThreadRoutineType(job0), &Job4Id); // 4 - long job
    Pool.doJob(PThreadRoutineType(job0), &Job5Id); // 5 - long job
    Pool.doJob(PThreadRoutineType(job0), &Job6Id); // 6 - long job

#ifndef WIN32
    timespec Delay;
    Delay.tv_sec = 0;
    Delay.tv_nsec = 100 * 1000 * 1000;
    nanosleep(&Delay, NULL);
#else
    Sleep(100);
#endif

    Pool.doJob(PThreadRoutineType(job1), &Job9Id); // 9 - short job
    Pool.doJob(PThreadRoutineType(job1), &Job10Id); // 10 - short job

#ifndef WIN32
    sleep(7);
#else
    Sleep(7000);
#endif
    cout << "-------------------------------" << endl;
    Pool.doJob(PThreadRoutineType(job0), &Job1Id); // 1 - long job
    Pool.doJob(PThreadRoutineType(job0), &Job2Id); // 2 - long job
    Pool.doJob(PThreadRoutineType(job0), &Job3Id); // 3 - long job
    Pool.doJob(PThreadRoutineType(job0), &Job4Id); // 4 - long job
    Pool.doJob(PThreadRoutineType(job0), &Job5Id); // 5 - long job
    Pool.doJob(PThreadRoutineType(job0), &Job6Id); // 6 - long job
#ifndef WIN32
    Delay.tv_sec = 0;
    Delay.tv_nsec = 100 * 1000 * 1000;
    nanosleep(&Delay, NULL);
#else
    Sleep(100);
#endif

    Pool.doJob(PThreadRoutineType(job1), &Job9Id); // 9 - short job
    Pool.doJob(PThreadRoutineType(job1), &Job10Id); // 10 - short job
#ifndef WIN32
    sleep(7);
#else
    Sleep(7000);
#endif

    cout << "-------------------------------" << endl;
    Pool.doJob(PThreadRoutineType(job0), &Job1Id); // 1 - long job
    Pool.doJob(PThreadRoutineType(job0), &Job2Id); // 2 - long job
    Pool.doJob(PThreadRoutineType(job0), &Job3Id); // 3 - long job
    Pool.doJob(PThreadRoutineType(job0), &Job4Id); // 4 - long job
    Pool.doJob(PThreadRoutineType(job0), &Job5Id); // 5 - long job
    Pool.doJob(PThreadRoutineType(job0), &Job6Id); // 6 - long job
#ifndef WIN32
    Delay.tv_sec = 0;
    Delay.tv_nsec = 200 * 1000 * 1000;
    nanosleep(&Delay, NULL);
#else
    Sleep(100);
#endif

    Pool.doJob(PThreadRoutineType(job1), &Job9Id); // 9 - short job
    Pool.doJob(PThreadRoutineType(job1), &Job10Id); // 10 - short job
#ifndef WIN32
    sleep(7);
#else
    Sleep(7000);
#endif

    cout << "-------------------------------" << endl;
    Pool.doJob(PThreadRoutineType(job0), &Job1Id); // 1 - long job
    Pool.doJob(PThreadRoutineType(job0), &Job2Id); // 2 - long job
    Pool.doJob(PThreadRoutineType(job0), &Job3Id); // 3 - long job
    Pool.doJob(PThreadRoutineType(job0), &Job4Id); // 4 - long job
    Pool.doJob(PThreadRoutineType(job0), &Job5Id); // 5 - long job
    Pool.doJob(PThreadRoutineType(job0), &Job6Id); // 6 - long job
#ifndef WIN32
    Delay.tv_sec = 0;
    Delay.tv_nsec = 100 * 1000 * 1000;
    nanosleep(&Delay, NULL);
#else
    Sleep(100);
#endif

    Pool.doJob(PThreadRoutineType(job1), &Job9Id); // 9 - short job
    Pool.doJob(PThreadRoutineType(job1), &Job10Id); // 10 - short job
#ifndef WIN32
    sleep(7);
#else
    Sleep(7000);
#endif

    return (EXIT_SUCCESS);
}

