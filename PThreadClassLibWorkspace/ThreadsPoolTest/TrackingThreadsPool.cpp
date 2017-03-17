#include <TrackingThreadsPool.h>

TrackingThreadsPool::TrackingThreadsPool(int NumberOfThreadsInPool, int BaseThreadPriority, int MonitoringPeriodMsecs, int ThreadTimeThreshold, int CancelationTimeMsecs,
		int* ProcessorNumbers, int NumOfProcessorsInSet) : PThreadsPool(NumberOfThreadsInPool, BaseThreadPriority),
    m_pMonitorThread(NULL), m_ThreadTimeThreshold(ThreadTimeThreshold), m_CancelationTimeMsecs(CancelationTimeMsecs)
{
	if(NumOfProcessorsInSet)
	{
#ifdef __SunOS
	    int PrSetResult = pset_create(&m_PoolProcessorsSet);
	    if(-1 == PrSetResult)
	    {
	        /// Error
	    }
	    else
	    {
//	        psetid_t RetSet;
	        for(int i = 0; i < NumOfProcessorsInSet; ++i)
	        {
	            PrSetResult = pset_assign(m_PoolProcessorsSet, ProcessorNumbers[i], NULL);
	        }
	        for(int i = 0; i < m_NumberOfThreadsInPool; ++i)
	        {
	            pset_bind(m_PoolProcessorsSet, P_LWPID, m_Threads[i]->getLwpId(), NULL);
	        }
	    }
#elif WIN32
            m_AffinityMask = 0;
            for(int i = 0; i < NumOfProcessorsInSet; ++i)
            {
                DWORD TmpMask = 1 << ProcessorNumbers[i];
                m_AffinityMask |= TmpMask;
            }
            for(int i = 0; i < m_NumberOfThreadsInPool; ++i)
            {
                    SetThreadAffinityMask(pthread_getw32threadhandle_np(m_Threads[i]->getThreadId()), m_AffinityMask);
            }
            m_BaseThreadPriority = THREAD_PRIORITY_NORMAL;
#elif __linux__
            CPU_ZERO(&m_AffinityMask);
            int i;
            for(i = 0; i < NumOfProcessorsInSet; ++i)
            {
                CPU_SET(ProcessorNumbers[i], &m_AffinityMask);
            }
            for(int i = 0; i < m_NumberOfThreadsInPool; ++i)
            {
                pthread_setaffinity_np(m_Threads[i]->getThreadId(), sizeof(m_AffinityMask), &m_AffinityMask);
            }
#endif
	}


    m_pMonitorThread = new PThreadClass(Runnable(PThreadRoutineType(monitoringRoutineRun), NULL, this), MonitoringPeriodMsecs, true);
#ifdef __linux__
    m_pMonitorThread->setSchedulingInheritance(PTHREAD_INHERIT_SCHED);
#endif
#ifndef WIN32
    m_pMonitorThread->setPriority(m_BaseThreadPriority + 1);
#else
   m_pMonitorThread->setPriority(THREAD_PRIORITY_ABOVE_NORMAL);
#endif
    m_pMonitorThread->run();
    m_pMonitorThread->setPolicy(SCHED_FIFO);
    m_pMonitorThread->start();
}

TrackingThreadsPool::~TrackingThreadsPool()
{
    if(m_pMonitorThread)
    {
        m_pMonitorThread->stop();
        m_pMonitorThread->exit();
        m_pMonitorThread->join();
        delete m_pMonitorThread;
        m_pMonitorThread = NULL;
    }
#ifdef __SunOS
    pset_destroy(m_PoolProcessorsSet);
#endif // __SunOS
}

void* TrackingThreadsPool::monitoringRoutine()
{
    for(int i = 0; i < m_NumberOfThreadsInPool; ++i)
    {
        if(m_Threads[i]->isWorking())
        {
#ifdef __SunOS
            hrtime_t LastRunCpuTimeMsecs;
#else
            u_int64_t LastRunCpuTimeMsecs;
#endif
            LastRunCpuTimeMsecs =  m_Threads[i]->getLastRunCpuTime() / 1000 / 1000;
            if(LastRunCpuTimeMsecs >= m_ThreadTimeThreshold && m_BaseThreadPriority == m_Threads[i]->getLastPriorityLevel())
            {
#ifndef WIN32
                m_Threads[i]->setPriority(m_BaseThreadPriority - 1);
#else
                m_Threads[i]->setPriority(THREAD_PRIORITY_BELOW_NORMAL);
#endif
            }
            if(m_CancelationTimeMsecs && LastRunCpuTimeMsecs >= m_CancelationTimeMsecs)
            {
                m_Threads[i]->cancelPoolThread();
                m_Threads[i]->setPolicy(SCHED_FIFO);
                m_Threads[i]->run();
                m_Threads[i]->setPriority(m_BaseThreadPriority);
                m_Threads[i]->setCancelType(PTHREAD_CANCEL_ASYNCHRONOUS);
#ifdef __SunOS
                pset_bind(m_PoolProcessorsSet, P_LWPID, m_Threads[i]->getLwpId(), NULL);
#elif WIN32
    		    SetThreadAffinityMask(pthread_getw32threadhandle_np(m_Threads[i]->getThreadId()), m_AffinityMask);
#elif __linux__
                pthread_setaffinity_np(m_Threads[i]->getThreadId(), sizeof(m_AffinityMask), &m_AffinityMask);
#endif
                m_pFreeThreadsMutex->lockMutex();
                m_FreeThreads.push(m_Threads[i]);
                m_pFreeThreadsMutex->unlockMutex();
                sem_post(&(m_ThreadsSemaphor));
            }
        }
    }
    return NULL;
}

