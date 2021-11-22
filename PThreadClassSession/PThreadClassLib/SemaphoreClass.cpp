/*
 * SemaphoreClass.cpp
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
#include "SemaphoreClass.h"
#ifdef _MSC_VER
#include "PThreadClass.h"
#endif

inline void addTimeMs(timespec& time, uint32_t time_addition)
{
    time.tv_nsec += (time_addition * 1000 * 1000);
    time.tv_sec += (time.tv_nsec / (1000 * 1000 * 1000));
    time.tv_nsec %= (1000 * 1000 * 1000);
}

#ifndef _WITHOUT_THREAD_EXCEPTIONS
void SemaphoreClass::init(uint32_t Count)
{
    if(m_Initialized)
    {
        THREAD_EXCEPT_THROW("The semaphore is initialized already.");
    }
    int32_t Result = 0;
    if(0 != (Result = sem_init(&m_Semaphore, 0, Count)))
    {
        THREAD_EXCEPT_THROW(errno);
    }
    m_Initialized = true;
}

void SemaphoreClass::post()
{
    if(!m_Initialized)
    {
        THREAD_EXCEPT_THROW("The semaphore is not initialized.");
    }
    int32_t Result = 0;
    if(0 != (Result = sem_post(&m_Semaphore)))
    {
        THREAD_EXCEPT_THROW(errno);
    }
}

int32_t SemaphoreClass::wait(uint32_t TimeoutMs)
{
    if(!m_Initialized)
    {
        THREAD_EXCEPT_THROW("The semaphore is not initialized.");
    }
    int32_t Result = 0;
    if(0 == TimeoutMs)
    {
        if(0 != (Result = sem_wait(&m_Semaphore)))
        {
            THREAD_EXCEPT_THROW(errno);
        }
        return Result;
    }
    else
    {
        timespec Now;
        clock_gettime(CLOCK_REALTIME, &Now);
        addTimeMs(Now, TimeoutMs);
        if(0 != (Result = sem_timedwait(&m_Semaphore, &Now)))
        {
            if(ETIMEDOUT != errno)
            {
                THREAD_EXCEPT_THROW(errno);
            }
            return ETIMEDOUT;
        }
    }
    return Result;
}

int32_t SemaphoreClass::trywait()
{
    if(!m_Initialized)
    {
        THREAD_EXCEPT_THROW("The semaphore is not initialized.");
    }
    int32_t Result = 0;
    if(0 != (Result = sem_trywait(&m_Semaphore)))
    {
        if(EAGAIN != errno)
        {
            THREAD_EXCEPT_THROW(errno);
        }
        return EAGAIN;
    }
    return Result;
}

int32_t SemaphoreClass::getvalue()
{
    if(!m_Initialized)
    {
        THREAD_EXCEPT_THROW("The semaphore is not initialized.");
    }
    int32_t Value = -1;
    int32_t Result = 0;
    if(0 != (Result = sem_getvalue(&m_Semaphore, &Value)))
    {
        THREAD_EXCEPT_THROW(errno);
    }
    return Value;
}
#else
bool SemaphoreClass::init(uint32_t Count)
{
    if(m_Initialized)
    {
        return false;
    }
    int32_t Result = 0;
    if(0 != (Result = sem_init(&m_Semaphore, 0, Count)))
    {
        return(errno);
    }
    m_Initialized = true;
    return 0;
}

int32_t SemaphoreClass::post()
{
    if(!m_Initialized)
    {
        return -1;
    }
    int32_t Result = 0;
    if(0 != (Result = sem_post(&m_Semaphore)))
    {
        return (errno);
    }
    return 0;
}

int32_t SemaphoreClass::wait(uint32_t TimeoutMs)
{
    if(!m_Initialized)
    {
        return -1;
    }
    int32_t Result = 0;
    if(0 == TimeoutMs)
    {
        if(0 != (Result = sem_wait(&m_Semaphore)))
        {
            return(errno);
        }
        return Result;
    }
    else
    {
        timespec Now;
        clock_gettime(CLOCK_REALTIME, &Now);
        addTimeMs(Now, TimeoutMs);
        if(0 != (Result = sem_timedwait(&m_Semaphore, &Now)))
        {
            if(ETIMEDOUT != errno)
            {
                return(errno);
            }
            return ETIMEDOUT;
        }
    }
    return Result;
}

int32_t SemaphoreClass::trywait()
{
    if(!m_Initialized)
    {
        return -1;
    }
    int32_t Result = 0;
    if(0 != (Result = sem_trywait(&m_Semaphore)))
    {
        if(EAGAIN != errno)
        {
            return(errno);
        }
        return EAGAIN;
    }
    return Result;
}

int32_t SemaphoreClass::getvalue()
{
    if(!m_Initialized)
    {
        return -1;
    }
    int32_t Value = -1;
    int32_t Result = 0;
    if(0 != (Result = sem_getvalue(&m_Semaphore, &Value)))
    {
        return(errno);
    }
    return Value;
}
#endif

