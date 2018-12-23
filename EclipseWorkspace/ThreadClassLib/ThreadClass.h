/* ThreadClass.h
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
// ThreadClass is wrapping for C++ threads with simple functionality          //
// The thread may be cyclic with some timeout or one shot thread              //
// The thread may also be attached or not.                                    //
////////////////////////////////////////////////////////////////////////////////

#ifndef THREADCLASS_H
#define THREADCLASS_H

#include "CondVarClass.h"
#include <thread>
#include <atomic>

namespace ThreadClassLib
{
/// @file
///
/// @defgroup LIB_GROUP Library Auxiliaries.
///
/// \{
/// Timeout duration zero constant.
#ifndef _WIN32
constexpr std::chrono::steady_clock::duration THREAD_IMMEDIATE(std::chrono::steady_clock::duration::zero());
#else
const std::chrono::steady_clock::duration THREAD_IMMEDIATE = std::chrono::steady_clock::duration::zero();
#endif
/// \}

///
/// @class ThreadClass
/// C++ thread wrapper class
/// @details Wrapper on C++ thread. The thread can be : <br/>
/// cyclic / one shot <br/>
/// attached / unattached <br/>
class THREADCLASSLIB_API ThreadClass
{
public:
    /// @struct ThreadRoutineType
    /// Thread routine base type (abstract).
    struct THREADCLASSLIB_API ThreadRoutineType : public std::unary_function<void, void>
	{
        ThreadRoutineType(ThreadClass* pThread) : m_pThread(pThread) {}
        ThreadRoutineType() : m_pThread(nullptr) {}
		virtual void operator()() = 0;
		ThreadClass* m_pThread; ///< For debug purposes
		/// Set running thread pointer for debug purposes
        virtual void setThread(ThreadClass* p);
        virtual ~ThreadRoutineType() {}
	};

    /// @struct ThreadRoutineType
	/// Thread stop notification base type (abstract).
    struct THREADCLASSLIB_API ThreadStopNotificatonType : public std::unary_function<void*, void>
	{
	    virtual void operator()(void*) = 0;
        virtual ~ThreadStopNotificatonType();
	};

    /// Constructor
	/// @param pThreadRoutine : poiter to thread routine functor.
    /// @param CyclicThread : cyclic (true) or one-shot (false) thread.
    /// @param Time : cyclic period value in milliseconds.
    /// @throw ThreadException
    ThreadClass(ThreadRoutineType* pThreadRoutine, bool CyclicThread = false, std::chrono::steady_clock::duration Time = THREAD_IMMEDIATE) :
            m_ControlCondVar(3), m_Cyclic(CyclicThread), m_Running(false), m_TimeOut(Time), m_ExitCondition(m_ControlCondVar, 0),
            m_StartCondition(m_ControlCondVar, 1), m_WaitCondition(m_ControlCondVar, 2), m_pThreadRoutine(pThreadRoutine), m_pThread(nullptr),
            m_Name(""), m_pStopNotificationFunc(nullptr), m_StopNotificationFuncArg(nullptr), m_InternalThreadRoutine(this)
    {}

    /// Destructor
    virtual ~ThreadClass()
    {
    	delete m_pThread;
    	m_pThread = nullptr;
    }

    /// Initialize and run thread, make it ready to execute thread routine.
    /// @throw ThreadException
    /// @returns false if thread cannot be created, true on success
    virtual bool run();

    /// Start execution of thread routine.
    /// \throw ThreadException
    virtual void start()
    {
        std::unique_lock<std::mutex>(m_StartCondition.getMutex());
        m_StartCondition.setPredicate(true);
        m_StartCondition.signal();
    }

    /// Stop cyclic execution of thread routine.
    virtual void stop()
    {
        m_Cyclic = false;
    }

    /// Destroy thread.
    /// \throw ThreadException
    virtual void exit()
    {
        std::unique_lock<std::mutex>(m_ExitCondition.getMutex());
        m_ExitCondition.setPredicate(true);
        m_ExitCondition.signal();
    }

    /// Join attached thread.
    virtual void join()
    {
        if(m_pThread)
        {
            m_pThread->join();
            delete m_pThread;
            m_pThread = nullptr;
        }
    }

    /// Is thread attached?
    /// \returns attached state.
    bool isAttached()
    {
        return m_pThread->joinable();
    }

    /// Is thread cyclic?
    /// \returns cyclic state.
    bool isCyclic()
    {
        bool Cyclic = m_Cyclic.load();
        return Cyclic;
    }

    /// Set thread cyclic state.
    /// \param On : true or false.
    void setCyclic(bool On)
    {
    	m_Cyclic = On;
    }

    /// Is thread ready for execution?
    /// \return running state.
    bool isRunning()
    {
    	std::unique_lock<std::mutex> Lock(m_ControlCondVar.getMutex());
        return m_Running;
    }

    /// Set cyclic period.
    /// \param TimeOutMs : cyclic period in milliseconds.
    void setTimeOut(std::chrono::milliseconds TimeOutMs)
    {
    	std::unique_lock<std::mutex> Lock(m_ControlCondVar.getMutex());
        m_TimeOut = TimeOutMs;
    }

    /// Get cyclic period value.
    /// \returns C++11 duration of cyclic period
    std::chrono::steady_clock::duration getTimeOut()
    {
    	std::unique_lock<std::mutex> Lock(m_ControlCondVar.getMutex());
        return m_TimeOut;
    }

    /// Get reference on internal condition variable.
    /// \returns reference on internal condition variable.
    CondVarClass& getConditionVar()
    {
        return m_ControlCondVar;
    }

    /// Name the thread.
    /// \param Name : thread name string.
    void nameThread(std::string Name)
    {
        m_Name = Name;
    }
    /// Get thread name.
    /// \returns Thread name string.
    std::string threadName()
    {
        return m_Name;
    }

    /// Registering of thread notification function. <br/>
    /// The function is actually called on main thread context, therefore should be as short as possible in runtime.
    /// \param Func : notification function pointer.
    /// \param Arg : notification function argument.
    void registerStopNotificationFunc(ThreadStopNotificatonType* pFunc, void* Arg)
    {
        m_pStopNotificationFunc = pFunc;
        m_StopNotificationFuncArg = Arg;
    }

    /// Set thread priority.
    /// \details should be called after run().
    /// \returns success or fail of the operation.
    /// \param Prio : priority level.
    bool setPriority(int Prio);

    /// Get priority level.
    /// \details should be called after run().
    /// \returns priority level or -1 on failure.
    int getPriority();

protected:
    CondVarClass m_ControlCondVar; ///< Internal condition variable.
    volatile std::atomic<bool> m_Cyclic; ///< Cyclic state.
    bool m_Running; ///< Running state.
    std::chrono::steady_clock::duration m_TimeOut; ///< Cyclic period value.
    SpecificCondition m_ExitCondition; ///< Specified exit condition variable.
    SpecificCondition m_StartCondition; ///< Specified start condition variable.
    SpecificCondition m_WaitCondition; ///< Specified wait condition variable.
    std::chrono::steady_clock::time_point m_StartTime; ///< Thread routine execution start time.
    ThreadRoutineType* m_pThreadRoutine; ///< Thread routine.
    std::thread* m_pThread; ///< C++ thread pointer.
    std::string m_Name; ///< Thread name.
    ThreadStopNotificatonType* m_pStopNotificationFunc; ///< Stop notification pointer.
    void* m_StopNotificationFuncArg; ///< Stop notification function argument.

    /// @struct InternalThreadRoutineType
    /// Internal Thread routine type derivation from abstract ThreadRoutineType.
    struct InternalThreadRoutineType : public ThreadRoutineType
    {
        InternalThreadRoutineType(ThreadClass* Thread) : ThreadRoutineType(Thread) {}
        virtual ~InternalThreadRoutineType();
        virtual void operator()()
        {
            if(m_pThread)
                (*m_pThread)();
        }
    };

    InternalThreadRoutineType m_InternalThreadRoutine; ///< Internal thread routine instance.

    /// Internal thread  routine.
   	void operator()();
};
} // end of ThreadClassLib namespace

#endif // THREADCLASS_H
