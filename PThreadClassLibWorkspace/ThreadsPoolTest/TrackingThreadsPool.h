/* 
 * File:   TrackingThreadsPool.h
 * Author: igor
 *
 * Created on November 11, 2009, 8:46 PM
 *
 * Since tracking event proccessing time could not be predicted in adavnce
 * some sofisticated CPU resources management is implemented in this class.
 * The class is threads pool (inherits from PThreadsPool) with CPU usage time
 * monitoring (by additional monitoring thread). When one of the pool's thread
 * overs defined CPU time usage threshold, its priority is lowered to allow
 * waiting threads (if exist) asquire CPU. To fully implement such kind of resource
 * management SCHED_FIFO policy should be appliyed. On MS Windows OS there is no
 * possibility for SCHED_FIFO and threads behavior is slightly different.
 *
 */


#ifndef _TRACKINGTHREADSPOOL_H
#define	_TRACKINGTHREADSPOOL_H


#include <PThreadsPool.h>

class TrackingThreadsPool : public PThreadsPool
{
public:
    TrackingThreadsPool(int NumberOfThreadsInPool, int BaseThreadPriority, int MonitoringPeriodMsecs, int ThreadTimeThreshold,
                        int CancelationTimeMsecs, int* ProcessorNumbers, int NumOfProcessorsInSet);
    ~TrackingThreadsPool();

    static void* monitoringRoutineRun(void* pContext)
    {
        return (reinterpret_cast<TrackingThreadsPool*>(pContext))->monitoringRoutine();
    }

    void* monitoringRoutine();

private:
    PThreadClass* m_pMonitorThread;
    unsigned int m_ThreadTimeThreshold;
    unsigned int m_CancelationTimeMsecs;
#ifdef __SunOS
    psetid_t m_PoolProcessorsSet;
#elif _WIN32
    DWORD m_AffinityMask;
#elif __linux__
    cpu_set_t m_AffinityMask;
#endif // __SunOS
};

#endif	/* _TRACKINGTHREADSPOOL_H */

