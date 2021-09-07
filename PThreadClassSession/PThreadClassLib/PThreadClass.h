/* PThreadClass.h
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
////////////////////////////////////////////////////////////////////////////////
// PThreadClass is wrapping for POSIX threads with simple functionality       //
// The thread may be cyclic with some timeout or one shot thread              //
// The thread may also be attached or not.                                    //
////////////////////////////////////////////////////////////////////////////////

#ifndef PTHREADCLASS_H
#define PTHREADCLASS_H

#include "CondVarClass.h"
#include <sched.h>

#ifndef _WIN32
#ifdef __VXWORKS__
#include <sys/times.h>
#else
#include <sys/time.h>
#endif
#else
#include <time.h>
struct timespec { long tv_sec; long tv_nsec; };
int clock_gettime(int, struct timespec *spec);
#endif

#include <sys/timeb.h>

/// \file 
///
/// \defgroup FUNC_DEFS Function types.
/// 
/// \{
extern "C"
{
    typedef void* (*PThreadRoutineType)(void*);
    typedef void (*PThreadCleanUpRoutineType) (void*);
}
/// \}

/// \struct Runnable
/// C++ wrapper on thread routine and clean-up routine.
struct PTHREADCLASSLIB_API Runnable
{
    /// Constructor
    /// \param pRoutine : pointer to thread routine.
    /// \param pCleanUpRoutine : pointer to clean-up routine.
    /// \param p : pointer to thread routine context.
    /// \param cp : pointer to clean-up routine context.
    /// \param CleanUpExecute : clean-up handler execution flag.
    Runnable(PThreadRoutineType pRoutine, PThreadCleanUpRoutineType pCleanUpRoutine = nullptr, void* p = nullptr, void* cp = nullptr, int CleanUpExecute = 0) : pthreadRoutine(pRoutine),
        cleanUpRoutine(pCleanUpRoutine), pContext(p), pCleanUpContext(cp), m_CleanUpExecute(CleanUpExecute)
    {}
    /// Cast to thread routine operator.
    operator PThreadRoutineType() // Cast operator
    {
        return pthreadRoutine;
    }
    
    /// Cast to clean-up routine operator.
    operator PThreadCleanUpRoutineType() // Cast operator
    {
        return cleanUpRoutine;
    }
    /// Function operator.
    void* operator()()
    {
        return pthreadRoutine(pContext);
    }

    PThreadRoutineType pthreadRoutine; ///< Thread routine.
    PThreadCleanUpRoutineType cleanUpRoutine; ///< Clean-up routine.
    void* pContext; ///< Thread routine context.
    void* pCleanUpContext; ///< Clean-up routine context.
    int m_CleanUpExecute; ///< Clean-up handler execution flag.
};

///
/// \class PThreadClass
/// POSIX thread class
/// \details Wrapper on POSIX thread. The thread can be : <br/>
/// cyclic / on shot <br/>
/// attached / unattached <br/>
/// bound / unbound to LWP (for OSs supporting this)
class PTHREADCLASSLIB_API PThreadClass
{
public:
    /// Constructor
    /// \param Routine : routines structure reference.
    /// \param Time : cyclic period value in milliseconds.
    /// \param CyclicThread : cyclic (true) or one-shot (false) thread.
    /// \param AttachedThread : attached (true) or unattached (false) thread.
    /// \param BoundThread : binding to LWP flag. Applicable only on supporting binding OSs.
    /// \throw ThreadException
    PThreadClass(Runnable Routine, time_t Time = PTHREAD_INFINITE, bool CyclicThread = false,
		bool AttachedThread = true
#ifdef __SunOS
    , bool BoundThread = false
#endif
        )
#ifndef _WITHOUT_THREAD_EXCEPTIONS
        noexcept(false)
#endif
        ;
    /// Destructor
    virtual ~PThreadClass();

#ifndef _WITHOUT_THREAD_EXCEPTIONS
    /// Initialize and run thread, make it ready to execute thread routine.
    /// \throw ThreadException
    virtual bool run() noexcept(false);
    /// Start execution of thread routine.
    /// \throw ThreadException
    virtual bool start() noexcept(false);
    /// Destroy thread.
    /// \throw ThreadException
    virtual bool exit() noexcept(false);
#else
    virtual int run();
    virtual bool start();
    virtual bool exit();
#endif
    /// Stop cyclic execution of thread routine.
    virtual bool stop();

    /// Join attached thread after.
    virtual bool join();
    
    /// Set thread priority.
    /// \return success or fail of the operation.
    /// \param Prio : POSIX priority level.
    bool setPriority(int Prio);
    
    /// Get POSIX priority level.
    /// \details should be called after run().
    /// \returns priority level or -1 on failure.
    int getPriority();
    
    /// Set thread scheduling policy.
    /// \details should be called before run().
    /// \return success or fail of the operation.
    /// \param Pol : POSIX scheduling policy.
    bool setPolicy(int Pol);
    
    /// Get POSIX priority level.
    /// \details should be called after run().
    /// \returns policy or -1 on failure.
    int getPolicy();
    
    /// Set thread stack address.
    /// \details should be called before run().
    /// \return success or fail of the operation.
    /// \param pStack : stack address.
    bool setStackAddr(void* pStack);
    
    /// Get thread stack address.
    /// \details should be called after run().
    /// \returns stack address or -1 on failure.
    void* getStackAddr();
    
    /// Set thread stack address.
    /// \details should be called before run().
    /// \return success or fail of the operation.
    /// \param pStack : stack address.
    bool setStackSize(size_t StackSize);
    
    /// Get thread stack size.
    /// \returns stack size or -1 on failure.
    size_t getStackSize();
    
    /// Is thread attached?
    /// \return attached state.
    bool isAttached()
    {
        return m_Attached;
    }
    
    /// Is thread cyclic?
    /// \return cyclic state.
    bool isCyclic()
    {
        return m_Cyclic;
    }
    
    /// Set thread cyclic state.
    /// \param On : true or false.
    void setCyclic(bool On);
    
    /// Is thread ready for execution?
    /// \return running state.
    bool isRunning()
    {
        return m_Running;
    }
    
    /// Set cyclic period.
    /// \param Time : cyclic period in milliseconds.
    void setTimeOut(time_t Time)
    {
        m_TimeOut = Time;
    }
    
    /// Get cyclic period value.
    /// \return cyclic period in milliseconds.
    time_t getTimeOut()
    {
        return m_TimeOut;
    }
    
    /// Get reference on internal condition variable.
    /// \return reference on internal condition variable.
    CondVarClass& getConditionVar()
    {
        return m_ControlCondVar;
    }
#ifdef _WITHOUT_THREAD_EXCEPTIONS
    bool isOk()
    {
        return m_Ok;
    }
#endif
    /// Get POSIX thread ID.
    /// \details should be called after run().
    /// \return POSIX thread ID.
    pthread_t getThreadId()
    {
        return m_ThreadId;
    }
    /// Scheduling policy inheritance setting
    /// \details should be called after run().
    /// \param InheritanceType : scheduling policy inheritance type PTHREAD_INHERIT_SCHED or PTHREAD_EXPLICIT_SCHED.
    int setSchedulingInheritance(int InheritanceType)
    {
        int Result = 0;
#if ( _POSIX_THREAD_PRIORITY_SCHEDULING >= 1 )
        Result = pthread_attr_setinheritsched(&m_ThreadAttribute, InheritanceType);
        if(Result != 0)
        {
            THREAD_EXCEPT_THROW(Result);
        }
#endif
        return Result;
    }
#ifdef __VXWORKS__
    int getTaskId()
    {
        return TaskId;
    }
#endif

#ifdef __SunOS
    /// Is thread bound t LWP?
    /// \return bound state.
    bool isBound()
    {
        return m_Bound;
    }
#endif
    void setCancelType(int CancelType = PTHREAD_CANCEL_DEFERRED)
    {
        m_CancelType = CancelType;
    }
    void cancel()
    {
        pthread_cancel(m_ThreadId);
    }

protected:
    pthread_t m_ThreadId; ///< POSIX thread ID.
    CondVarClass m_ControlCondVar; ///< Internal condition variable.
    Runnable m_ThreadRoutine; ///< Thread riotines structure.
    volatile bool m_Cyclic; ///< Cyclic state.
    bool m_Attached; ///< Attached state.
#ifdef __SunOS
    bool m_Bound; ///< Bound state.
#endif
    bool m_Running; ///< Running state.
    time_t m_TimeOut; ///< Cyclic period value.
    SpecificCondition* m_pExitCondition; ///< Scpecified exit condipion variable.
    SpecificCondition* m_pStartCondition; ///< Scpecified start condipion variable.
    SpecificCondition* m_pWaitCondition; ///< Scpecified wait condipion variable.
    pthread_attr_t m_ThreadAttribute; ///< POSIX thread attributes.
    struct timespec m_StartTime; ///< Start time of cycle.
#ifdef _WITHOUT_THREAD_EXCEPTIONS
    bool m_Ok;
#endif
#ifdef __VXWORKS__
    int TaskId;
#endif
    int m_CancelType;

    /// Thread launcher 
    static void* threadLauncher(void* pContext)
    {
        return (reinterpret_cast < PThreadClass* > (pContext))->threadControlRoutine();
    }
    /// Internal thread routine.
    virtual void* threadControlRoutine();
    /// Internal thread loop.
    virtual void* threadLoop();
};

#endif // PTHREADCLASS_H

