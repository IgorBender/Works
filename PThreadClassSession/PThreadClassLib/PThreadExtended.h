/* PThreadExtended.h
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

#ifndef PTHREADEXTENDED_H
#define	PTHREADEXTENDED_H

#include "PThreadClass.h"

/// \file 
///
/// \defgroup FUNC_DEFS Function types.
/// 
/// \{
extern "C"
{
    typedef void (*PThreadNotificationFuncType) (void*);
}
/// \}


///
/// \class PThreadExtended
/// POSIX thread class with extended info
/// \details Wrapper on PThreadClass. 
class PTHREADCLASSLIB_API PThreadExtended : public PThreadClass
{
public:
    /// Constructor
    /// \param Routine : routines structure reference.
    /// \param Time : cyclic period value in milliseconds.
    /// \param CyclicThread : cyclic (true) or one-shot (false) thread.
    /// \param AttachedThread : attached (true) or unattached (false) thread.
    /// \param BoundThread : binding to LWP flag. Applicable only on supporting binding OSs.
    /// \throw ThreadException
    PThreadExtended(Runnable Routine, time_t Time = PTHREAD_INFINITE, bool CyclicThread = false,
		bool AttachedThread = true
#ifdef __SunOS
    , bool BoundThread = false
#endif
        ) : PThreadClass(Routine, Time, CyclicThread, AttachedThread
#ifdef __SunOS
    , BoundThread
#endif
        ), m_Name(""), m_StopNotificationFunc(nullptr), m_StopNotificationFuncArg(nullptr)
    {}

    /// Initialize and run thread, make it ready to execute thread routine.
    /// \throw ThreadException
#ifndef _WITHOUT_THREAD_EXCEPTIONS
    virtual bool run() noexcept(false);
#else
    virtual int run();
#endif
    /// Name the thread.
    /// \param Name : thread name string.
    void nameThread(string Name);
    /// Get thread name.
    /// \return Thread name string.
    string threadName()
    {
        return m_Name;
    }
    /// Registering of thread notification function. <br/>
    /// The function is actually called on main thread context, therefore should be as short as possible in runtime.
    /// \param Func : notification function pointer.
    /// \param Arg : notification function argument.
    void registerStopNotificationFunc(PThreadNotificationFuncType Func, void* Arg)
    {
        m_StopNotificationFunc = Func;
        m_StopNotificationFuncArg = Arg;
    }
    
private:
    string m_Name; /// Thread name.
    PThreadNotificationFuncType m_StopNotificationFunc; /// Stop notification function pointer.
    void* m_StopNotificationFuncArg; /// Stop notification function argument.
    
    /// Internal thread loop.
    virtual void* threadLoop();
    /// Name the thread.
    void nameThread();
};

#endif	/* PTHREADEXTENDED_H */

