/*
 *  CondVarCountedClass.cpp
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
#include "CondVarCountedClass.h"
#include <time.h>

#ifndef _MSC_VER
#include <sys/time.h>
#else
#include <time.h>
#include <sys/timeb.h>
#endif

#ifndef _WITHOUT_THREAD_EXCEPTIONS
CondVarCountedClass::CondVarCountedClass()
{
    int Result;
    pthread_mutexattr_t MutexAttr;
    Result = pthread_mutexattr_init(&MutexAttr);
    if(Result != 0)
    {
        THREAD_EXCEPT_THROW(Result);
    }
#if ( _POSIX_THREAD_PRIO_INHERIT >= 1 ) && !(__linux__)
    Result = pthread_mutexattr_setprotocol(&MutexAttr, PTHREAD_PRIO_INHERIT);
    if(Result != 0)
    {
        THREAD_EXCEPT_THROW(Result);
    }
#endif
    Result = pthread_mutex_init(&m_Mutex, &MutexAttr);
    if(Result != 0)
    {
        THREAD_EXCEPT_THROW(Result);
    }
    pthread_mutexattr_destroy(&MutexAttr);
    Result = pthread_cond_init(&m_CondVar, nullptr);
    if(Result != 0)
    {
        THREAD_EXCEPT_THROW(Result);
    }
}
#else
CondVarCountedClass::CondVarCountedClass()
{
    int Result;
    pthread_mutexattr_t MutexAttr;
    Result = pthread_mutexattr_init(&MutexAttr);
    if(Result != 0)
    {
        pthread_mutexattr_destroy(&MutexAttr);
        Ok = false;
        return;
    }
#if ( _POSIX_THREAD_PRIO_INHERIT >= 1 ) && !(__linux__)
    Result = pthread_mutexattr_setprotocol(&MutexAttr, PTHREAD_PRIO_INHERIT);
    if(Result != 0)
    {
        pthread_mutexattr_destroy(&MutexAttr);
        Ok = false;
        return;
    }
#endif
    Result = pthread_mutex_init(&m_Mutex, &MutexAttr);
    if(Result != 0)
    {
        pthread_mutexattr_destroy(&MutexAttr);
        Ok = false;
        return;
    }
    pthread_mutexattr_destroy(&MutexAttr);
    Result = pthread_cond_init(&m_CondVar, nullptr);
    if(Result != 0)
    {
        Ok = false;
        return;
    }
    Ok = true;
}
#endif

CondVarCountedClass::~CondVarCountedClass()
{
    pthread_mutex_unlock(&m_Mutex);
    pthread_mutex_destroy(&m_Mutex);
    pthread_cond_destroy(&m_CondVar);
}

int CondVarCountedClass::wait(time_t Timeout)
{
    int Result;
    struct timespec Time;
#ifdef _MSC_VER
    struct timeb TimeStruct;
#endif
    if(Timeout)
    {
#ifdef _MSC_VER
        ftime(&TimeStruct);
        TimeStruct.time += (Timeout / 1000);
        TimeStruct.millitm += static_cast < short > (Timeout % 1000);
        Time.tv_nsec = TimeStruct.millitm * 1000 * 1000;
        Time.tv_sec = static_cast < long > (TimeStruct.time);
#else
        int ClockRes = clock_gettime(CLOCK_REALTIME, &Time);
        if(ClockRes < 0)
            return EINVAL;
        Time.tv_sec += Timeout / 1000; // Add seconds
        Time.tv_nsec += (Timeout % 1000) * 1000 * 1000; // Nanoseconds
        if(Time.tv_nsec > 1000000000)
        {
            Time.tv_sec++;
            Time.tv_nsec -= 1000000000;
        }
#endif
        Result = pthread_cond_timedwait(&m_CondVar, &m_Mutex, &Time);
        if(Result == 0)
        {
            return Result;
        }
        else if(Result == ETIMEDOUT)
        {
            return Result;
        }
        else
        {
            return EINVAL;
        }
    }
    else
    {
        Result = pthread_cond_wait(&m_CondVar, &m_Mutex);
        if(Result == 0)
        {
            return Result;
        }
        else
        {
            return EINVAL;
        }
    }
//    return 0;
}

