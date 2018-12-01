/* ThreadClass.cpp
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
#include "ThreadClass.h"
#include <iostream>
#ifndef WIN32
#include <unistd.h>
#include <pthread.h>
#else
#include <Windows.h>
#endif

namespace ThreadClassLib
{
void ThreadClass::ThreadRoutineType::setThread(ThreadClass* p)
{
    m_pThread = p;
}

void ThreadClass::operator()()
{
    while(1)
    {
        std::unique_lock<std::mutex> Lock(m_ControlCondVar.getMutex());
        if(m_ExitCondition.getPredicate())
        {
            m_ExitCondition.setPredicate(false);
            m_Running = false;
            return;
        }
        if(m_StartCondition.getPredicate())
        {
            m_StartCondition.setPredicate(false);
            if(Lock.owns_lock())
                Lock.unlock();
            m_StartTime = std::chrono::steady_clock::now();
            m_Running = true;
            m_pThreadRoutine->operator()();
            m_Running = false;
            bool Cyclic = m_Cyclic.load();
            if(Cyclic)
            {
                auto Now = std::chrono::steady_clock::now();
                if(THREAD_IMMEDIATE != m_TimeOut && THREAD_IMMEDIATE < m_StartTime + m_TimeOut - Now)
                {
                    m_WaitCondition.wait(m_StartTime - Now + m_TimeOut);
                }
                Lock.lock();
                m_StartCondition.setPredicate(true);
                m_StartCondition.signal();
                Lock.unlock();
                continue;
            }
            else
            {
                if(Lock.owns_lock())
                    Lock.unlock();
                if(m_pStopNotificationFunc)
                {
                    m_pStopNotificationFunc->operator()(m_StopNotificationFuncArg);
                }
            }
        }
        if(m_ExitCondition.getPredicate())
        {
            m_ExitCondition.setPredicate(false);
            m_Running = false;
            return;
        }
        if(m_StartCondition.getPredicate())
        {
            if(Lock.owns_lock())
                Lock.unlock();
            continue;
        }
        if(Lock.owns_lock())
            Lock.unlock();
        int WaitResult = m_ControlCondVar.wait();
        if(WaitResult != 0)
        {
            if(Lock.owns_lock())
                Lock.unlock();
            continue;
        }
    }
}

bool ThreadClass::setPriority(int Prio)
{
    if(m_pThread)
    {
        std::thread::native_handle_type NativeHandle = m_pThread->native_handle();
#ifndef WIN32
#if (_POSIX_THREAD_PRIORITY_SCHEDULING >= 1)
        sched_param Params;
        bzero(reinterpret_cast < char* > (&Params), sizeof Params);
        int TmpPolicy;
        if(pthread_getschedparam(NativeHandle, &TmpPolicy, &Params) != 0)
        {
            return false;
        }
        Params.sched_priority = Prio;
        if(pthread_setschedparam(NativeHandle, TmpPolicy, &Params) == 0)
            return true;
        else
            return false;
#endif
#else
        return TRUE == SetThreadPriority(NativeHandle, Prio) ? true : false;
#endif
    }
    return false;
}

int ThreadClass::getPriority()
{
    if(m_pThread)
    {
        std::thread::native_handle_type NativeHandle = m_pThread->native_handle();
#ifndef WIN32
#if (_POSIX_THREAD_PRIORITY_SCHEDULING >= 1)
        sched_param Params;
        int TmpPolicy;
        if(pthread_getschedparam(NativeHandle, &TmpPolicy, &Params) != 0)
        {
            return -1;
        }
        return Params.sched_priority;
#endif
#else
        int Prio;
        if(THREAD_PRIORITY_ERROR_RETURN == (Prio = GetThreadPriority(NativeHandle)))
        {
            return -1;
        }
        return Prio;
#endif
    }
    return -1;
}
} // end of ThreadClassLib namespace
