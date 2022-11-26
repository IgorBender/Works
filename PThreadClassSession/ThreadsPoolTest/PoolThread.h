/* 
 * File:   PoolThread.h
 * Author: igor
 *
 * Created on November 5, 2009, 5:12 AM
 *
 * Thread class that is element of threads pool, executes registered routine
 * with registered argument, provides methods to retrieve overall used CPU time
 * and used CPU time of last run.
 * On Solaris OS provides LWP ID.
 *
 */

#ifndef _POOLTHREAD_H
#define	_POOLTHREAD_H

#include <PThreadClass.h>
#ifdef __SunOS
#include <sys/resource.h>
#include <sys/lwp.h>
#include <procfs.h>
#endif
#ifdef __linux__
#include <sys/types.h>
#include <syscall.h>
#define UTIME_ENTRY 14
#endif
#ifdef _MSC_VER
typedef __int64 u_int64_t;
#endif

class PoolThread : public PThreadClass
{
public:
    PoolThread(unsigned int Id);
    ~PoolThread();

    unsigned int getPoolThreadId()
    {
        return m_Id;
    }
    void registerRoutine(PThreadRoutineType Routine)
    {
        m_pRoutine = Routine;
    }
    void registerRutinesAgrument(void* pArg)
    {
        m_pArgument = pArg;
    }

    void registerCleanUp(PThreadCleanUpRoutineType CleanUpRoutine)
    {
        m_pCleanUpRoutine = CleanUpRoutine;
    }

    void registerCleanUpAgrument(void* pArg)
    {
        m_pCleanUpArg = pArg;
    }

    void resetRoutine()
    {
        m_pRoutine = NULL;
    }
    void resetArgument()
    {
        m_pArgument = NULL;
    }

    void resetCleanUpRoutine()
    {
        m_pCleanUpRoutine = NULL;
    }

    void resetCleanUpArgument()
    {
        m_pCleanUpArg = NULL;
    }

    static void* threadMainRoutineRun(void* pContext)
    {
        return (reinterpret_cast<PoolThread*>(pContext))->threadMainRoutine();
    }

    static void cleanUpRoutineRun(void* pContext)
    {
        return (reinterpret_cast<PoolThread*>(pContext))->cleanUpRoutine();
    }

    void* threadMainRoutine();
    void cleanUpRoutine();

#ifdef __SunOS
    hrtime_t getLastRunCpuTime();
    hrtime_t getOverallRunCpuTime();
#else
    u_int64_t getLastRunCpuTime();
    u_int64_t getOverallRunCpuTime();
#endif

#ifdef __SunOS
    static const intptr_t ThreadTimeOffset = (intptr_t) (&((prusage_t *) (NULL))->pr_utime);
    static const intptr_t ThreadTimeSize = (intptr_t) (&((prusage_t *) (NULL))->pr_ttime) - (intptr_t) (&((prusage_t *) (NULL))->pr_utime);

    lwpid_t getLwpId()
    {
        return m_LwpId;
    }
#endif

    bool setPriority(int Prio)
    {
        if(PThreadClass::setPriority(Prio))
        {
            m_LastPriorityLevel = Prio;
            return true;
        }
        return false;
    }

    int getLastPriorityLevel()
    {
        return m_LastPriorityLevel;
    }

    /*
     * Cancelation type shall be set before calling of start() method of the thread.
     * The defined cancelation type will be accepted by the thread at calling of start().
     */
    void setCancelType(int CancelType = PTHREAD_CANCEL_DEFERRED)
    {
        m_CancelType = CancelType;
    }
    void cancelPoolThread()
    {
        cancel();
        m_Working = false;
        m_Running = false;
#ifndef _MSC_VER
        bzero(m_ProcessName, sizeof(m_ProcessName));
#endif
    }
    bool isWorking()
    {
    	return m_Working;
    }

protected:
#ifdef __SunOS
    lwpid_t m_LwpId;
//    string m_ProcessName;
    char m_ProcessName[128];
#elif (__linux__)
    unsigned int m_ThreadPid;
    char m_ProcessName[128];
    unsigned int m_TicksInSecond;
#endif
    virtual void* threadControlRoutine()
    {
#ifdef __SunOS
        m_LwpId = _lwp_self();
#elif (__linux__)
        m_ProcessName[0] = '\0';
        m_ThreadPid = syscall(SYS_gettid);
#endif
        return PThreadClass::threadControlRoutine();
    }

private:
    PThreadRoutineType m_pRoutine;
    void* m_pArgument;
    PThreadCleanUpRoutineType m_pCleanUpRoutine;
    void* m_pCleanUpArg;
    unsigned int m_Id;
#ifdef __SunOS
    hrtime_t m_LastCpuTime;
    hrtime_t m_OverallCpuTime;
#else
    u_int64_t m_LastCpuTime;
//    long long m_CurrentCpuTime;
    u_int64_t m_OverallCpuTime;
#endif
    int m_LastPriorityLevel;
    bool m_Working;
};

#endif	/* _POOLTHREAD_H */

