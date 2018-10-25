/* PThreadClass.cpp
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

#include <PThreadClass.h>
#if (!defined(_WIN32) &&  !defined(__VXWORKS__))
#include <strings.h>
#include <signal.h>
#endif

PThreadClass::PThreadClass(Runnable Routine, unsigned long Time, bool CyclicThread, bool AttachedThread
#ifdef __SunOS
    , bool BoundThread
#endif
    )
#ifndef _WITHOUT_THREAD_EXCEPTIONS
noexcept(false)
#endif
        :
        m_ControlCondVar(0),
        m_ThreadRoutine(Routine),
        m_Cyclic(CyclicThread),
        m_Attached(AttachedThread),
        m_Running(false),
#ifdef __SunOS
        m_Bound(BoundThread),
#endif
        m_TimeOut(Time),
        m_pExitCondition(NULL),
        m_pStartCondition(NULL),
        m_pWaitCondition(NULL),
        m_CancelType(PTHREAD_CANCEL_DEFERRED)
{
#ifdef _WITHOUT_THREAD_EXCEPTIONS
    m_Ok = true;
#endif
#ifdef __VXWORKS__
    TaskId = 0;
#endif
#ifndef _WIN32
    bzero(reinterpret_cast < char* > (&m_ThreadId), sizeof m_ThreadId);
#else
    memset(&m_ThreadId, 0, sizeof m_ThreadId);
#endif
    m_pExitCondition = new SpecificCondition(m_ControlCondVar, m_ControlCondVar.addPredicate(false));
    m_pStartCondition = new SpecificCondition(m_ControlCondVar, m_ControlCondVar.addPredicate(false));
    m_pWaitCondition = new SpecificCondition(m_ControlCondVar, m_ControlCondVar.addPredicate(false));
    int Result = pthread_attr_init(&m_ThreadAttribute);
    if(Result != 0)
    {
        THREAD_EXCEPT_THROW(Result);
#ifdef _WITHOUT_THREAD_EXCEPTIONS
        m_Ok = false;
#endif
    }
    Result = pthread_attr_setdetachstate(&m_ThreadAttribute, m_Attached ? PTHREAD_CREATE_JOINABLE : PTHREAD_CREATE_DETACHED);
    if(Result != 0)
    {
        THREAD_EXCEPT_THROW(Result);
#ifdef _WITHOUT_THREAD_EXCEPTIONS
        m_Ok = false;
#endif
    }
#if ( _POSIX_THREAD_PRIORITY_SCHEDULING >= 1 )
    Result = pthread_attr_setinheritsched(&m_ThreadAttribute, PTHREAD_EXPLICIT_SCHED);
    if(Result != 0)
    {
        THREAD_EXCEPT_THROW(Result);
    }
//    Result = pthread_attr_setschedpolicy(&m_ThreadAttribute, SCHED_FIFO);
//    if(Result != 0)
//    {
//        THREAD_EXCEPT_THROW(Result);
//    }
#endif
#ifdef __SunOS
    Result = pthread_attr_setscope(&m_ThreadAttribute, m_Bound ? PTHREAD_SCOPE_SYSTEM : PTHREAD_SCOPE_PROCESS);
    if(Result != 0)
    {
        THREAD_EXCEPT_THROW(Result);
    }
#endif
#ifdef _WITHOUT_THREAD_EXCEPTIONS
    m_Ok = m_Ok && m_pExitCondition->isOk() && m_pStartCondition->isOk() && m_pWaitCondition->isOk() && (Result == 0);
#endif
}

PThreadClass::~PThreadClass()
{
    pthread_attr_destroy(&m_ThreadAttribute);
    if(m_pWaitCondition)
    {
        delete m_pWaitCondition;
        m_pWaitCondition = NULL;
    }
    if(m_pStartCondition)
    {
        delete m_pStartCondition;
        m_pStartCondition = NULL;
    }
    if(m_pExitCondition)
    {
        delete m_pExitCondition;
        m_pExitCondition = NULL;
    }
}

void* PThreadClass::threadControlRoutine()
{
    void* pReturn;
    if(m_ThreadRoutine.cleanUpRoutine)
    {
        pthread_cleanup_push(PThreadCleanUpRoutineType(m_ThreadRoutine), m_ThreadRoutine.pCleanUpContext);
        pReturn = this->threadLoop();
        pthread_cleanup_pop(m_ThreadRoutine.m_CleanUpExecute);
    }
    else
    {
        pReturn = threadLoop();
    }
    return pReturn;
}

void* PThreadClass::threadLoop()
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
            return NULL;
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
            ftime(&m_StartTime);
            int Dummy;
            if(PTHREAD_CANCEL_ASYNCHRONOUS == m_CancelType)
            {
                pthread_setcanceltype(m_CancelType, &Dummy);
            }
            m_ThreadRoutine();
            if(PTHREAD_CANCEL_ASYNCHRONOUS == m_CancelType)
            {
                pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &Dummy);
            }
            if(m_Cyclic)
            {
                struct timeb TimeStruct;
                ftime(&TimeStruct);
                if(m_TimeOut && m_TimeOut > static_cast < unsigned > (((TimeStruct.time - m_StartTime.time) * 1000 + (TimeStruct.millitm - m_StartTime.millitm))))
                {
                    m_pWaitCondition->lockMutex();
                    m_pWaitCondition->wait(m_TimeOut - ((TimeStruct.time - m_StartTime.time) * 1000 + (TimeStruct.millitm - m_StartTime.millitm)));
                    m_pWaitCondition->unlockMutex();
                }
                m_pStartCondition->lockMutex();
                m_pStartCondition->setPredicate(true);
                m_pStartCondition->unlockMutex();
                continue;
            }
        }

#ifndef _WITHOUT_THREAD_EXCEPTIONS
        if(m_pExitCondition->getPredicate())
#else
        m_pExitCondition->getPredicate(StartPred);
        if(StartPred)
#endif
        {
            m_ControlCondVar.unlockMutex();
            return NULL;
        }
#ifndef _WITHOUT_THREAD_EXCEPTIONS
        if(m_pStartCondition->getPredicate())
#else
        m_pStartCondition->getPredicate(StartPred);
        if(StartPred)
#endif
        {
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
    m_Running = false;
    return NULL;
}

#ifndef _WITHOUT_THREAD_EXCEPTIONS
bool PThreadClass::run() noexcept(false)
#else
int PThreadClass::run()
#endif
{
    if(m_Running)
#ifndef _WITHOUT_THREAD_EXCEPTIONS
        return false;
#else
    return -1;
#endif

    int Result = pthread_create(&m_ThreadId, &m_ThreadAttribute,
                                reinterpret_cast < PThreadRoutineType > (PThreadClass::threadLauncher), this);
    if(Result != 0)
    {
#ifndef _WITHOUT_THREAD_EXCEPTIONS
        THREAD_EXCEPT_THROW(Result);
#else
        return Result;
#endif
    }
    sched_yield();
    m_Running = true;
#ifdef _WITHOUT_THREAD_EXCEPTIONS
    return Result;
#endif
    return true;
}

#ifndef _WITHOUT_THREAD_EXCEPTIONS
bool PThreadClass::start() noexcept(false)
#else
bool PThreadClass::start()
#endif
{
    if(!m_Running)
        return false;
    m_pWaitCondition->lockMutex();
    m_pWaitCondition->setPredicate(false);
    m_pWaitCondition->unlockMutex();
    m_pStartCondition->lockMutex();
    m_pStartCondition->setPredicate(true); // An exception may be thrown
    m_pStartCondition->signal();
    m_pStartCondition->unlockMutex();
    sched_yield();
    return true;
}

bool PThreadClass::stop()
{
    if(!m_Running)
        return false;

    setCyclic(false);
    sched_yield();
    return true;
}

#ifndef _WITHOUT_THREAD_EXCEPTIONS
bool PThreadClass::exit() noexcept(false)
#else
bool PThreadClass::exit()
#endif
{
    m_pExitCondition->lockMutex();
    m_pExitCondition->setPredicate(true); // An exception may be thrown
    m_pExitCondition->signal();
    m_pExitCondition->unlockMutex();
    return true;
}

bool PThreadClass::join()
{
    if(m_Attached)
    {
        char* pChar;
        return !pthread_join(m_ThreadId, (void**)&pChar);
    }
    return true;
}

bool PThreadClass::setPriority(int Prio)
{
#if ( _POSIX_THREAD_PRIORITY_SCHEDULING  >= 1 ) || defined(_WIN32)
    sched_param Params;
#ifndef _WIN32
    bzero(reinterpret_cast < char* > (&Params), sizeof Params);
#else
    memset(&Params, 0, sizeof Params);
#endif
    
#if (__linux__)
    pthread_t Dummy; // Empty thread
    bzero(reinterpret_cast < char* > (&Dummy), sizeof Dummy);
    if(pthread_equal(Dummy, m_ThreadId) || 0 != pthread_kill(m_ThreadId, 0))
#else
    if(pthread_kill(m_ThreadId, 0) != 0)
#endif
    {
        Params.sched_priority = Prio;
        if(pthread_attr_setschedparam(&m_ThreadAttribute, &Params) == 0)
        {
            if(pthread_attr_setinheritsched(&m_ThreadAttribute, PTHREAD_EXPLICIT_SCHED) == 0)
                return true;
            else
                return false;
        }
        else
            return false;
    }
    else
    {
        int TmpPolicy;
        if(pthread_getschedparam(m_ThreadId, &TmpPolicy, &Params) != 0)
        {
            return false;
        }
        Params.sched_priority = Prio;
        if(pthread_setschedparam(m_ThreadId, TmpPolicy, &Params) == 0)
            return true;
        else
            return false;
    }        
#endif
    return true;
}

int PThreadClass::getPriority()
{
#if (__linux__)
    pthread_t Dummy; // Empty thread
    bzero(reinterpret_cast < char* > (&Dummy), sizeof Dummy);
    if(pthread_equal(Dummy, m_ThreadId) || 0 != pthread_kill(m_ThreadId, 0))
#else
    if(pthread_kill(m_ThreadId, 0) != 0)
#endif
        return -1;
    
    sched_param Params;
#ifndef _WIN32
    bzero(reinterpret_cast < char* > (&Params), sizeof Params);
#else
    memset(&Params, 0, sizeof Params);
#endif
#if ( _POSIX_THREAD_PRIORITY_SCHEDULING >= 1 ) || defined(_WIN32)
    int TmpPolicy;
    if(pthread_getschedparam(m_ThreadId, &TmpPolicy, &Params) != 0)
    {
        return -1;
    }
    return Params.sched_priority;
#endif
    return -1;
}

bool PThreadClass::setPolicy(int Pol)
{
#if ( _POSIX_THREAD_PRIORITY_SCHEDULING >= 1 )
#if (__linux__)
    pthread_t Dummy; // Empty thread
    bzero(reinterpret_cast < char* > (&Dummy), sizeof Dummy);
    if(pthread_equal(Dummy, m_ThreadId) || 0 != pthread_kill(m_ThreadId, 0))
#else
    if(pthread_kill(m_ThreadId, 0) != 0)
#endif
    {
        if(pthread_attr_setschedpolicy(&m_ThreadAttribute, Pol) == 0)
        {
            if(pthread_attr_setinheritsched(&m_ThreadAttribute, PTHREAD_EXPLICIT_SCHED) == 0)
                return true;
            else
                return false;
        }
        else
            return false;
    }
    else
    {
        sched_param Params;
#ifndef _WIN32
        bzero(reinterpret_cast < char* > (&Params), sizeof Params);
#else
        memset(&Params, 0, sizeof Params);
#endif
        int Policy;
        if(pthread_getschedparam(m_ThreadId, &Policy, &Params) != 0)
        {
            return false;
        }
        Policy = Pol;
        if(pthread_setschedparam(m_ThreadId, Policy, &Params) == 0)
            return true;
        else
            return false;
    }        
#endif
    return true;
}

int PThreadClass::getPolicy()
{
#if (__linux__)
    pthread_t Dummy; // Empty thread
    bzero(reinterpret_cast < char* > (&Dummy), sizeof Dummy);
    if(pthread_equal(Dummy, m_ThreadId) || 0 != pthread_kill(m_ThreadId, 0))
#else
    if(pthread_kill(m_ThreadId, 0) != 0)
#endif
        return -1;
    
    sched_param Params;
#ifndef _WIN32
    bzero(reinterpret_cast < char* > (&Params), sizeof Params);
#else
    memset(&Params, 0, sizeof Params);
#endif
#if ( _POSIX_THREAD_PRIORITY_SCHEDULING >= 1 )
    int Policy;
    if(pthread_getschedparam(m_ThreadId, &Policy, &Params) != 0)
    {
        return -1;
    }
    return Policy;
#endif
    return -1;
}

bool PThreadClass::setStackAddr(void* pStack)
{
#if !( _POSIX_THREAD_ATTR_STACKADDR >= 1 )
    return false;
#endif
#if (__linux__)
    pthread_t Dummy; // Empty thread
    bzero(reinterpret_cast < char* > (&Dummy), sizeof Dummy);
    if(!pthread_equal(Dummy, m_ThreadId) && 0 == pthread_kill(m_ThreadId, 0))
#else
    if(pthread_kill(m_ThreadId, 0) == 0)
#endif
        return false;
#ifdef _WIN32
    if(pthread_attr_setstackaddr(&m_ThreadAttribute, pStack) != 0)
        return false;
    else
        return true;
#else
    void* pTmpAddr;
    size_t Size;
    if(pthread_attr_getstack(&m_ThreadAttribute, &pTmpAddr, &Size) != 0)
        return  false;
    if(pthread_attr_setstack(&m_ThreadAttribute, pStack, Size) != 0)
        return false;
    else
        return true;
    
#endif
    return true;
}

void* PThreadClass::getStackAddr()
{
#if !( _POSIX_THREAD_ATTR_STACKADDR >= 1 )
    return false;
#endif
    void* TmpPtr = 0;
#ifdef _WIN32
    if(pthread_attr_getstackaddr(&m_ThreadAttribute, &TmpPtr) != 0)
        return NULL;
    else
        return TmpPtr;
#else
    size_t Size;
    if(pthread_attr_getstack(&m_ThreadAttribute, &TmpPtr, &Size) != 0)
        return  NULL;
    else
        return TmpPtr;
    
#endif
    return NULL;
}

bool PThreadClass::setStackSize(size_t StackSize)
{
#if !( _POSIX_THREAD_ATTR_STACKSIZE >= 1 )
    return false;
#endif
#if (__linux__)
    pthread_t Dummy; // Empty thread
    bzero(reinterpret_cast < char* > (&Dummy), sizeof Dummy);
    if(!pthread_equal(Dummy, m_ThreadId) && 0 == pthread_kill(m_ThreadId, 0))
#else
    if(pthread_kill(m_ThreadId, 0) == 0)
#endif
        return false;
    if(pthread_attr_setstacksize(&m_ThreadAttribute, StackSize) != 0)
        return false;
    else
        return true;
    return true;
}

size_t PThreadClass::getStackSize()
{
#if !( _POSIX_THREAD_ATTR_STACKSIZE >= 1 )
    return false;
#endif
    size_t TmpSize = 0;
    if(pthread_attr_getstacksize(&m_ThreadAttribute, &TmpSize) != 0)
        return static_cast < size_t > (-1);
    else
        return TmpSize;
    return static_cast < size_t > (-1);
}

void PThreadClass::setCyclic(bool On)
{
    m_Cyclic = On;
}
