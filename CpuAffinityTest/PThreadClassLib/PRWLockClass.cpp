/* PRWLockClass.cpp
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

#include "PRWLockClass.h"
#include <time.h>
#ifdef _MSC_VER
#include <sys/timeb.h>
#endif

#ifndef _WITHOUT_THREAD_EXCEPTIONS
PRWLockClass::PRWLockClass() noexcept(false)
{
    int Result;
#ifdef __VXWORKS__
    bzero(reinterpret_cast < char* > (&m_RWLock), sizeof m_RWLock);
#endif
    pthread_rwlockattr_t LockAttr;
#ifdef __VXWORKS__
    bzero(reinterpret_cast < char* > (&LockAttr), sizeof LockAttr);
#endif
    Result = pthread_rwlockattr_init(&LockAttr);
    if(Result != 0)
    {
        THREAD_EXCEPT_THROW(Result);
    }
#ifdef _MSC_VER
    memset(&LockAttr, 0, sizeof LockAttr);
#endif
    Result = pthread_rwlock_init(&m_RWLock, &LockAttr);
    if(Result != 0)
    {
        THREAD_EXCEPT_THROW(Result);
    }
    pthread_rwlockattr_destroy(&LockAttr);
}
#else
PRWLockClass::PRWLockClass()
{
    int Result;
#ifdef __VXWORKS__
    bzero(reinterpret_cast < char* > (&m_RWLock), sizeof m_RWLock);
#endif
    pthread_rwlockattr_t LockAttr;
#ifdef __VXWORKS__
    bzero(reinterpret_cast < char* > (&LockAttr), sizeof LockAttr);
#endif
    Result = pthread_rwlockattr_init(&LockAttr);
    if(Result != 0)
    {
        pthread_rwlockattr_destroy(&LockAttr);
        Ok = false;
        return;
    }
#ifdef _MSC_VER
    memset(&LockAttr, 0, sizeof LockAttr);
#endif
    Result = pthread_rwlock_init(&m_RWLock, &LockAttr);
    if(Result != 0)
    {
        pthread_rwlockattr_destroy(&LockAttr);
        Ok = false;
        return;
    }
    pthread_rwlockattr_destroy(&LockAttr);
    Ok = true;
}
#endif

PRWLockClass::~PRWLockClass()
{
    pthread_rwlock_unlock(&m_RWLock);
    pthread_rwlock_destroy(&m_RWLock);
}

int PRWLockClass::timedLockRead(unsigned long Timeout)
{
    int Result;
    struct timespec Time;
#ifdef WIN32
    struct timeb TimeStruct;
#endif
    if(Timeout)
    {
#ifdef WIN32
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
        Result = pthread_rwlock_timedrdlock(&m_RWLock, &Time);
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
        Result = pthread_rwlock_rdlock(&m_RWLock);
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

int PRWLockClass::timedLockWrite(unsigned long Timeout)
{
    int Result;
    struct timespec Time;
#ifdef WIN32
    struct timeb TimeStruct;
#endif
    if(Timeout)
    {
#ifdef WIN32
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
        Result = pthread_rwlock_timedwrlock(&m_RWLock, &Time);
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
        Result = pthread_rwlock_wrlock(&m_RWLock);
        if(Result == 0)
        {
            return Result;
        }
        else
        {
            return EINVAL;
        }
    }
    return 0;
}
