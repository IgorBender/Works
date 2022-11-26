/* CondVarClass.h
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

#ifndef CONVARCLASS_H
#define CONVARCLASS_H

#include <pthread.h>
#include <errno.h>

#include "ThreadException.h"
#include <bitset>

#ifdef _MSC_VER
#pragma warning ( disable : 4290 )
#else
#include <unistd.h>
#endif

//#define PTHREAD_INFINITE 0UL

///
/// \class CondVarClass
/// Wrapping on POSIX condition variable.
class PTHREADCLASSLIB_API CondVarClass {
public:
/// \typedef Predicate type
typedef uint32_t PredicateIdType;
/// \define 
#define MAX_NUM_OF_PREDS 32
#ifndef _WITHOUT_THREAD_EXCEPTIONS
    /// Constructor.
    /// \param PredNum : number of predicates.
    CondVarClass(uint32_t PredNum) noexcept(false);

    /// Add predicate to predicates set.
    /// \param PredicateValue : value of adding predicate.
    /// \throw ThreadException
    PredicateIdType addPredicate(bool PredicateValue) noexcept(false);
    /// Get value of the specified predicate.
    /// \return Predicate value.
    /// \param PredNum : predicate number.
    /// \throw ThreadException
    bool getPredicate(PredicateIdType PredNum) noexcept(false);
    /// Set value of the specified predicate.
    /// \param PredNum : predicate number.
    /// \param PredicateValue : predicate value.
    /// \throw ThreadException
    void setPredicate(PredicateIdType PredNum, bool PredicateValue) noexcept(false);
#else
    CondVarClass(PredicateIdType PredNum);

    PredicateIdType addPredicate(bool PredicateValue);
    bool getPredicate(PredicateIdType PredNum, bool& PredicateValue);
    bool setPredicate(PredicateIdType PredNum, bool PredicateValue);
    bool isOk()
    {
        return Ok;
    }
#endif

    /// Destructor
    virtual ~CondVarClass();

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

    /// Signal condition variable .
    /// \return result of signal operation (0 - success).
    int signal()
    {
        return(pthread_cond_signal(&m_CondVar));
    }

    /// Broadcast condition variable.
    /// \return result of broadcast operation (0 - success).
    int broadcast()
    {
        return(pthread_cond_broadcast(&m_CondVar));
    }

    /// Wait for condition variable to be signaled.
    /// \return result of wait : <br/>
    ///     0 - condition variable is signaled.<br/>
    ///     ETIMEDOUT - wait timeout expired.<br/>
    ///     other - error.
    int wait(time_t Timeout = PTHREAD_INFINITE);

    /// Get number of associated predicates.
    /// \return number of predicates.
    PredicateIdType getPredNum()
    {
        return m_PredNum;
    }

protected:
    bitset < MAX_NUM_OF_PREDS > m_Predicates; ///< Associated predicates set.
    PredicateIdType m_PredNum; ///< Number of associated predicates.
    pthread_cond_t m_CondVar; ///< POSIX condition variable.
    pthread_mutex_t m_Mutex; ///< Associated mutual exclusive lock.
#ifdef _WITHOUT_THREAD_EXCEPTIONS
    bool Ok;
#endif
};

///
/// \class SpecificCondition
/// Wrapper on specific predicate of conditopn variable.
class PTHREADCLASSLIB_API SpecificCondition {
public:
#ifndef _WITHOUT_THREAD_EXCEPTIONS
    /// Constructor
    /// \param Cond : condition variable object reference.
    /// \param Index : number of specific predicate.
    SpecificCondition(CondVarClass& Cond, CondVarClass::PredicateIdType Index) noexcept(false);

    /// Get value of the specified predicate.
    /// \return Predicate value.
    /// \throw ThreadException
    bool getPredicate() noexcept(false)
    {
        return m_CondVarCl.getPredicate(m_Index);
    }
    /// Set value of the specified predicate.
    /// \param PredicateValue : predicate value.
    /// \throw ThreadException
    void setPredicate(bool PredicateValue) noexcept(false)
    {
        m_CondVarCl.setPredicate(m_Index, PredicateValue);
    }
#else
    SpecificCondition(CondVarClass& Cond, CondVarClass::PredicateIdType Index);
    
    bool getPredicate(bool& PredicateValue)
    {
        return m_CondVarCl.getPredicate(m_Index, PredicateValue);
    }
    bool setPredicate(bool PredicateValue)
    {
        return m_CondVarCl.setPredicate(m_Index, PredicateValue);
    }
    bool isOk()
    {
        return Ok;
    }
#endif

    /// Lock condition variable associated mutual exclusive lock.
    /// \return result of lock operation (0 - success).
    int lockMutex()
    {
        return m_CondVarCl.lockMutex();
    }

    /// Try to lock condition variable associated mutual exclusive lock.
    /// \return result of lock operation (0 - success).
    int tryLockMutex()
    {
        return m_CondVarCl.tryLockMutex();
    }

    /// Unlock condition variable associated mutual exclusive lock.
    /// \return result of unlock operation (0 - success).
    int unlockMutex()
    {
        return m_CondVarCl.unlockMutex();
    }

    /// Signal condition variable .
    /// \return result of signal operation (0 - success).
    int signal()
    {
        return m_CondVarCl.signal();
    }
    /// Broadcast condition variable.
    /// \return result of broadcast operation (0 - success).
    int broadcast()
    {
        return m_CondVarCl.broadcast();
    }
    /// Wait for condition variable to be signaled.
    /// \return result of wait : <br/>
    ///         0 - condition variable is signaled.<br/>
    ///         ETIMEDOUT - wait timeout expired.<br/>
    ///         other - error.<br/>
    int wait(time_t Timeout = PTHREAD_INFINITE)
    {
        return m_CondVarCl.wait(Timeout);
    }

protected:
    CondVarClass& m_CondVarCl; ///< Condition variable object reference.
    CondVarClass::PredicateIdType m_Index; ///< Specified predicate number.
#ifdef _WITHOUT_THREAD_EXCEPTIONS
    bool Ok;
#endif
};

#endif // _CONVARCLASS_H

