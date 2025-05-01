#include <PoolThread.h>

#include <fcntl.h>
#include <sstream>
#ifndef _MSC_VER
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#endif


/*
 * The thread should be bound th a LWP on Solaris OS.
 *
 */
PoolThread::PoolThread(unsigned int Id) : PThreadClass(Runnable(PThreadRoutineType(threadMainRoutineRun), PThreadCleanUpRoutineType(cleanUpRoutineRun), this, this),
                                                       PTHREAD_INFINITE,
                                                       false, // Cyclic
                                                       true   // Attached
#ifdef __SunOS
                                                       , true
#endif
                                                       ),
#ifdef __SunOS
                                                       m_LwpId(-1),
#elif __linux__
                                                       m_ThreadPid(static_cast<uint32_t>(-1)),
#endif
                                                       m_pRoutine(nullptr), m_pArgument(nullptr), m_pCleanUpRoutine(nullptr), m_pCleanUpArg(nullptr), m_Id(Id), m_LastCpuTime(0), m_OverallCpuTime(0),
                                                       m_LastPriorityLevel(0), m_Working(false)
{
#ifdef __SunOS
    bzero(m_ProcessName, sizeof(m_ProcessName));
//#else
//    memset(m_ProcessName, 0, sizeof(m_ProcessName));
#endif
#ifdef __linux__
#if ( _POSIX_THREAD_PRIORITY_SCHEDULING >= 1 )
    int Result = pthread_attr_setinheritsched(&m_ThreadAttribute, PTHREAD_INHERIT_SCHED);
    if(Result != 0)
    {
        THREAD_EXCEPT_THROW(Result);
    }
    m_TicksInSecond = uint32_t(sysconf(_SC_CLK_TCK));
#endif
#endif
}

PoolThread::~PoolThread()
{}

void* PoolThread::threadMainRoutine()
{
    m_Working = true;
    if(m_pRoutine)
    {
        void* ReturnValue = m_pRoutine(m_pArgument);
        m_OverallCpuTime = getOverallRunCpuTime();
        m_LastCpuTime = m_OverallCpuTime;
        m_Working = false;
        return ReturnValue;
    }
    m_Working = false;
    return nullptr;
}

void PoolThread::cleanUpRoutine()
{
    if(m_pCleanUpRoutine)
    {
        m_pCleanUpRoutine(m_pCleanUpArg);
        m_Working = false;
#ifndef _MSC_VER
		bzero(m_ProcessName, sizeof(m_ProcessName));
#endif
    }
}

#ifdef __SunOS
hrtime_t PoolThread::getOverallRunCpuTime()
#else
u_int64_t PoolThread::getOverallRunCpuTime()
#endif
{
#ifdef __SunOS
//    if(-1 != m_LwpId && m_ProcessName.empty())
    if(-1 != m_LwpId && 0 == strlen(m_ProcessName))
    {
        char LwpStr[16];
        sprintf(m_ProcessName, "/proc/self/lwp/%d/lwpusage", m_LwpId);

//        strcpy(m_ProcessName, "/proc/self/lwp/");
//        sprintf(LwpStr, "%d", m_LwpId);
//        strcat(m_ProcessName, LwpStr);
//        strcat(m_ProcessName, "/lwpusage");
//        stringstream BuildString;
//        BuildString << "/proc/self/lwp/" << m_LwpId << "/lwpusage";
//        m_ProcessName = BuildString.str();
    }

//    int fd = open(m_ProcessName.c_str(), O_RDONLY);
    int fd = open(m_ProcessName, O_RDONLY);
    if(fd == -1)
    {
        return -1;
    }

    int Count;
    prusage_t ProcUsage;
    do
    {
        Count = pread(fd, (void *) & ProcUsage.pr_utime,
                      PoolThread::ThreadTimeSize, PoolThread::ThreadTimeOffset);
    } while(Count < 0 && errno == EINTR);
    close(fd);
    if(Count < 0)
    {
        return -1;
    }

    hrtime_t LwpTime = (((hrtime_t) ProcUsage.pr_stime.tv_sec +
        (hrtime_t) ProcUsage.pr_utime.tv_sec) * 1000000000) +
        (hrtime_t) ProcUsage.pr_stime.tv_nsec +
        (hrtime_t) ProcUsage.pr_utime.tv_nsec;

    m_OverallCpuTime = LwpTime;
    return (LwpTime);
#endif // __SunOS
#ifdef _MSC_VER
	FILETIME m_StartTime;
	FILETIME ExitTime;
	FILETIME KernelCpuTime;
	FILETIME UserCpuTime;
	if(!GetThreadTimes(pthread_getw32threadhandle_np(m_ThreadId), &m_StartTime, &ExitTime, &KernelCpuTime, &UserCpuTime))
	{
		return -1;
	}
	ULARGE_INTEGER KernelTime;
	ULARGE_INTEGER UserTime;
	KernelTime.HighPart = KernelCpuTime.dwHighDateTime;
	KernelTime.LowPart = KernelCpuTime.dwLowDateTime;
	UserTime.HighPart = UserCpuTime.dwHighDateTime;
	UserTime.LowPart = UserCpuTime.dwLowDateTime;
    m_OverallCpuTime = (KernelTime.QuadPart + UserTime.QuadPart) * 100;
    return m_OverallCpuTime;
#endif
#ifdef __linux__
    if(0 == m_ProcessName[0])
    {
    	sprintf(m_ProcessName, "/proc/%u/task/%u/stat", getpid(), m_ThreadPid);
    }
    int fd = open(m_ProcessName, O_RDONLY);
    if(0 > fd)
    {
        return static_cast<uint32_t>(-1);
    }
    char readBuffer[256];
    ssize_t n;
    n = read(fd, readBuffer, 256);
    if(0 < n)
        readBuffer[n - 1] = '\0';
    close(fd);
    char* str = readBuffer;
    char* savePtr;
    for(int i = 0; i < UTIME_ENTRY - 1; ++i, str = nullptr)
    {
        char* token = strtok_r(str, " ", &savePtr);
        if(token == nullptr)
        {
            return static_cast<uint32_t>(-1);
        }
    }
    char* token = strtok_r(str, " ", &savePtr);
    uint64_t utime = uint64_t(atoi(token));
    token = strtok_r(str, " ", &savePtr);
    uint64_t stime = uint64_t(atoi(token));

    if(token == nullptr)
    {
        return static_cast<uint32_t>(-1);
    }
    m_OverallCpuTime = ((utime + stime)  * 1000 * 1000 * 1000) / m_TicksInSecond;
    return m_OverallCpuTime;
#endif
}


#ifdef __SunOS
hrtime_t PoolThread::getLastRunCpuTime()
#else
u_int64_t PoolThread::getLastRunCpuTime()
#endif
{
#ifdef __linux__
    u_int64_t TmpTime = getOverallRunCpuTime();
    if(m_LastCpuTime > TmpTime)
    {
        m_LastCpuTime = TmpTime;
        return 0;
    }
    u_int64_t ResultTime = TmpTime - m_LastCpuTime;
    return ResultTime;
#else
    return getOverallRunCpuTime() - m_LastCpuTime;
#endif
}
