/* PThreadExtended.cpp
 *
 * Original code by Igor Bender
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must
 * not claim that you wrote the original software. If you use this
 * software in a product, an acknowledgment in the product documentation
 * would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 */

#include "PThreadExtended.h"
#ifndef _MSC_VER
#include <unistd.h>
#include <pthread.h>
#else
#include <Windows.h>
#endif

#ifdef _MSC_VER
void nameThreadWindows(DWORD t, const char* name)
{
#pragma pack(push,8)
    typedef struct tagTHREADNAME_INFO
    {
        DWORD dwType; // Must be 0x1000.
        LPCSTR szName; // Pointer to name (in user addr space).
        DWORD dwThreadID; // Thread ID (-1=caller thread).
        DWORD dwFlags; // Reserved for future use, must be zero.
    } THREADNAME_INFO;
#pragma pack(pop)

    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = (LPCSTR)name;
    info.dwThreadID = t;
    info.dwFlags = 0;
    const DWORD MS_VC_EXCEPTION = 0x406D1388;
    __try
    {
        RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
    }

}
#endif

void* PThreadExtended::threadLoop()
{
#ifdef __VXWORKS__
    TaskId = taskIdSelf();
#endif
    while(1)
    {
        m_ControlCondVar.lockMutex();
#ifndef _WITHOUT_THREAD_EXCEPTIONS
        if(m_pExitCondition->getPredicate())
#else
        static bool ExitPred = false;
        m_pExitCondition->getPredicate(ExitPred);
        if(ExitPred)
#endif
        {
            m_pExitCondition->setPredicate(false);
            m_ControlCondVar.unlockMutex();
            m_Running = false;
            return nullptr;
        }
#ifndef _WITHOUT_THREAD_EXCEPTIONS
        if(m_pStartCondition->getPredicate())
#else
        static bool StartPred = false;
        m_pStartCondition->getPredicate(StartPred);
        if(StartPred)
#endif
        {
            m_pStartCondition->setPredicate(false);
            m_ControlCondVar.unlockMutex();
            clock_gettime(CLOCK_MONOTONIC, &m_StartTime);
            m_ThreadRoutine();
            if(m_Cyclic)
            {
                struct timespec TimeStruct;
                clock_gettime(CLOCK_MONOTONIC, &TimeStruct);
                if(m_TimeOut && m_TimeOut > static_cast < unsigned > (((TimeStruct.tv_sec - m_StartTime.tv_sec) * 1000 + (TimeStruct.tv_nsec - m_StartTime.tv_nsec) / 1000 / 1000)))
                {
                    m_pWaitCondition->lockMutex();
                    m_pWaitCondition->wait(m_TimeOut - ((TimeStruct.tv_sec - m_StartTime.tv_sec) * 1000 + (TimeStruct.tv_nsec - m_StartTime.tv_nsec) / 1000 / 1000));
                    m_pWaitCondition->unlockMutex();
                }
                m_pStartCondition->lockMutex();
                m_pStartCondition->setPredicate(true);
                m_pStartCondition->unlockMutex();
                continue;
            }
			else
			{
				if(m_StopNotificationFunc)
				{
					m_StopNotificationFunc(m_StopNotificationFuncArg);
				}
			}
            m_ControlCondVar.lockMutex();
        }

#ifndef _WITHOUT_THREAD_EXCEPTIONS
        if(m_pExitCondition->getPredicate())
#else
        m_pExitCondition->getPredicate(StartPred);
        if(StartPred)
#endif
        {
            m_ControlCondVar.unlockMutex();
            return nullptr;
        }
#ifndef _WITHOUT_THREAD_EXCEPTIONS
        if(m_pStartCondition->getPredicate())
#else
        m_pStartCondition->getPredicate(StartPred);
        if(StartPred)
#endif
        {
            m_ControlCondVar.unlockMutex();
            continue;
        }
        int WaitResult = m_ControlCondVar.wait();
        if(WaitResult == 0)
        {
            m_ControlCondVar.unlockMutex();
            continue;
        }
        m_ControlCondVar.unlockMutex();
    }
//    m_Running = false;
//    return nullptr;
}

#ifndef _WITHOUT_THREAD_EXCEPTIONS
bool PThreadExtended::run() noexcept(false)
#else
int PThreadExtended::run()
#endif
{
#ifndef _WITHOUT_THREAD_EXCEPTIONS
    bool Result = PThreadClass::run();
#else
    int Result = PThreadClass::run();
    return -1;
#endif
    nameThread();
    return Result;
}

void PThreadExtended::nameThread(string Name)
{
    m_Name = Name;
    if(m_Running)
    {
        if("" != m_Name)
#ifndef _MSC_VER
            pthread_setname_np(m_ThreadId, m_Name.c_str());
#else
            nameThreadWindows(GetThreadId(pthread_getw32threadhandle_np(m_ThreadId)), m_Name.c_str());
#endif
    }
}

void PThreadExtended::nameThread()
{
    if(m_Running)
    {
        if("" != m_Name)
#ifndef _MSC_VER
            pthread_setname_np(m_ThreadId, m_Name.c_str());
#else
            nameThreadWindows(GetThreadId(pthread_getw32threadhandle_np(m_ThreadId)), m_Name.c_str());
#endif
    }
}

