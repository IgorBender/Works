/*
 *  CondVarCountedClass.h
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
#ifndef CONDVARCOUNTEDCLASS_H
#define CONDVARCOUNTEDCLASS_H

#include <pthread.h>
#include <errno.h>

#include "ThreadException.h"

#ifndef _MSC_VER
#include <unistd.h>
#endif

///
/// \class CondVarCountedClass
/// Wrapping on POSIX condition variable.
class PTHREADCLASSLIB_API CondVarCountedClass
{
public:
    /// Constructor.
#ifndef _WITHOUT_THREAD_EXCEPTIONS
    CondVarCountedClass() noexcept(false);;
#else
    CondVarCountedClass();

    bool isOk()
    {
        return Ok;
    }
#endif
    /// Destructor
    virtual ~CondVarCountedClass();

    /// Get value of the internal counter, should be protected by mutex lock.
    /// \return Counter value.
    int32_t getConter()
    {
        return m_Counter;
    }

    /// Increment value of the internal counter, should be protected by mutex lock.
    void incrementCouner()
    {
        ++m_Counter;
    }

    /// Decrement value of the internal counter, should be protected by mutex lock.
    void decrementCouner()
    {
        if(0 < m_Counter)
            --m_Counter;
    }

    /// Lock condition variable associated mutual exclusive lock.
    /// \return result of lock operation (0 - success).
    int lockMutex()
    {
        return (pthread_mutex_lock(&m_Mutex));
    }

    /// Try to lock condition variable associated mutual exclusive lock.
    /// \return result of lock operation (0 - success).
    int tryLockMutex()
    {
        return pthread_mutex_trylock(&m_Mutex);
    }

    /// Unlock condition variable associated mutual exclusive lock.
    /// \return result of unlock operation (0 - success).
    int unlockMutex()
    {
        return pthread_mutex_unlock(&m_Mutex);
    }

    /// Signal condition variable.
    /// \return result of signal operation (0 - success).
    int signal()
    {
        return(pthread_cond_signal(&m_CondVar));
    }

    /// Wait for condition variable to be signaled.
    /// \return result of wait : <br/>
    ///     0 - condition variable is signaled.<br/>
    ///     ETIMEDOUT - wait timeout expired.<br/>
    ///     other - error.
    int wait(time_t Timeout = PTHREAD_INFINITE);

protected:
    int32_t m_Counter = 0; ///< Condition variable signal counter.
    pthread_cond_t m_CondVar; ///< POSIX condition variable.
    pthread_mutex_t m_Mutex; ///< Associated mutual exclusive lock.
#ifdef _WITHOUT_THREAD_EXCEPTIONS
    bool Ok;
#endif
};

#endif // CONDVARCOUNTEDCLASS_H
