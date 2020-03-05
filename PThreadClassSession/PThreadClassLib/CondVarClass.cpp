/* CondVarClass.cpp
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
#include "CondVarClass.h"
#include <time.h>

#ifndef _WIN32
#ifdef __VXWORKS__
#include <sys/times.h>
#else
#include <sys/time.h>
#endif
#else
#include <time.h>
#include <sys/timeb.h>
#endif

#ifndef _WITHOUT_THREAD_EXCEPTIONS
CondVarClass::CondVarClass(uint32_t PredNum) noexcept(false) : m_PredNum(PredNum)
{
    if(PredNum > MAX_NUM_OF_PREDS)
    {
        THREAD_EXCEPT_THROW("Too much predicates");
    }
    int Result;
#ifdef __VXWORKS__
    bzero(reinterpret_cast < char* > (&m_Mutex), sizeof m_Mutex);
#endif
    pthread_mutexattr_t MutexAttr;
#ifdef __VXWORKS__
    bzero(reinterpret_cast < char* > (&MutexAttr), sizeof MutexAttr);
#endif
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
#ifdef __VXWORKS__
    bzero(reinterpret_cast < char* > (&m_CondVar), sizeof m_CondVar);
#endif
    Result = pthread_cond_init(&m_CondVar, nullptr);
    if(Result != 0)
    {
        THREAD_EXCEPT_THROW(Result);
    }
    m_Predicates.reset();
}

bool CondVarClass::getPredicate(PredicateIdType PredNum) noexcept(false)
{
    if(m_PredNum <= PredNum)
    {
        THREAD_EXCEPT_THROW("Wrong predicate index");
    }
    return m_Predicates[PredNum];
}

void CondVarClass::setPredicate(PredicateIdType PredNum, bool PredicateValue) noexcept(false)
{
    if(m_PredNum <= PredNum)
    {
        THREAD_EXCEPT_THROW("Wrong predicate index");
    }
    m_Predicates[PredNum] = PredicateValue;
}

CondVarClass::PredicateIdType CondVarClass::addPredicate(bool PredicateValue) noexcept(false)
{
    if(m_PredNum >= MAX_NUM_OF_PREDS)
    {
        THREAD_EXCEPT_THROW("Too much predicates");
    }
    m_Predicates[m_PredNum++] = PredicateValue;
    return m_PredNum - 1;
}
#else
CondVarClass::CondVarClass(PredicateIdType PredNum) : m_PredNum(PredNum)
{
    if(PredNum > MAX_NUM_OF_PREDS)
    {
        Ok = false;
        return;
    }
    int Result;
#ifdef __VXWORKS__
    bzero(reinterpret_cast < char* > (&m_Mutex), sizeof m_Mutex);
#endif
    pthread_mutexattr_t MutexAttr;
#ifdef __VXWORKS__
    bzero(reinterpret_cast < char* > (&MutexAttr), sizeof MutexAttr);
#endif
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
#ifdef __VXWORKS__
    bzero(reinterpret_cast < char* > (&m_CondVar), sizeof m_CondVar);
#endif
    Result = pthread_cond_init(&m_CondVar, nullptr);
    if(Result != 0)
    {
        Ok = false;
        return;
    }
    m_Predicates.reset();
    Ok = true;
}

bool CondVarClass::getPredicate(PredicateIdType PredNum, bool& PredicateValue)
{
    if(m_PredNum <= PredNum)
    {
        return false;
    }
    PredicateValue = m_Predicates[PredNum];
    return true;
}

bool CondVarClass::setPredicate(PredicateIdType PredNum, bool PredicateValue)
{
    if(m_PredNum <= PredNum)
    {
        return false;
    }
    m_Predicates[PredNum] = PredicateValue;
    return true;
}

CondVarClass::PredicateIdType CondVarClass::addPredicate(bool PredicateValue)
{
    if(m_PredNum >= MAX_NUM_OF_PREDS - 1)
    {
        return static_cast<uint32_t>(-1);
    }
    m_Predicates[m_PredNum++] = PredicateValue;
    return m_PredNum - 1;
}
#endif
CondVarClass::~CondVarClass()
{
    pthread_mutex_unlock(&m_Mutex);
    pthread_mutex_destroy(&m_Mutex);
    pthread_cond_destroy(&m_CondVar);
}

int CondVarClass::wait(time_t Timeout)
{
    int Result;
    struct timespec Time;
#ifdef _WIN32
//    struct timeval CurrTime;
    struct timeb TimeStruct;
#endif
    if(Timeout)
    {
#ifdef _WIN32
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


#ifndef _WITHOUT_THREAD_EXCEPTIONS
SpecificCondition::SpecificCondition(CondVarClass& Cond, CondVarClass::PredicateIdType Index) noexcept(false)
        : m_CondVarCl(Cond), m_Index(Index)
{
    if(Cond.getPredNum() <= Index)
    {
        THREAD_EXCEPT_THROW("Wrong predicate index");
    }
}
#else
SpecificCondition::SpecificCondition(CondVarClass& Cond, CondVarClass::PredicateIdType Index) : m_CondVarCl(Cond), m_Index(Index)
{
    Ok = true;
    if(Cond.getPredNum() <= Index)
    {
        Ok = false;
        return;
    }
}
#endif

