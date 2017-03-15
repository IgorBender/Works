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

#ifndef _CONVARCLASS_H
#define _CONVARCLASS_H

#include <cstdint>
#include <condition_variable>
#include <mutex>
#include <memory>
//#include <errno.h>

#include "ThreadException.h"

#ifdef _WIN32
#pragma warning ( disable : 4290 )
#else
//#include <unistd.h>
#endif

namespace ThreadClassLib
{
///
/// \class CondVarClass
/// Wrapping on C++ condition variable.
class THREADCLASSLIB_API CondVarClass {
public:
/// \typedef Predicate type
typedef unsigned int PredicateIdType;

/// \define 
#define MAX_NUM_OF_PREDS 32

    /// Constructor.
    /// \param PredNum : number of predicates (up to 32).
    CondVarClass(int PredNum) throw(ThreadException);
    /// Destructor
    virtual ~CondVarClass() {}

    /// Add predicate to predicates set.
    /// \returns Number of added predicate.
    /// \param PredicateValue : value of predicate to add.
    /// \throw ThreadException
    PredicateIdType addPredicate(bool PredicateValue) throw(ThreadException);
    /// Get value of the specified predicate.
    /// \returns Predicate value.
    /// \param PredNum : predicate number.
    /// \throw ThreadException
    bool getPredicate(PredicateIdType PredNum) throw(ThreadException);
    /// Set value of the specified predicate.
    /// \param PredNum : predicate number.
    /// \param PredicateValue : predicate value.
    /// \throw ThreadException
    void setPredicate(PredicateIdType PredNum, bool PredicateValue) throw(ThreadException);

    /// Signal condition variable .
    void signal();

    /// Broadcast condition variable.
    void broadcast();

    /// Wait for condition variable to be signaled.
    /// \returns Predicates after wait.
    uint32_t wait(std::chrono::steady_clock::duration Timeout = std::chrono::steady_clock::duration::zero());

    /// Get number of associated predicates.
    /// \return number of predicates.
    PredicateIdType getPredNum()
    {
        return m_PredNum;
    }

    /// Get internal mutex.
    /// \return reference to internal mutex.
    std::mutex& getMutex()
    {
    	return m_Mutex;
    }

protected:
    uint32_t m_Predicates; ///< Associated predicates set.
    PredicateIdType m_PredNum; ///< Number of associated predicates.
    std::condition_variable m_CondVar; ///< C++ condition variable.
    std::mutex m_Mutex; ///< Associated mutual exclusive lock.
};

///
/// \class SpecificCondition
/// Wrapper on specific predicate of conditopn variable.
class THREADCLASSLIB_API SpecificCondition {
public:
    /// Constructor
    /// \param Cond : condition variable object reference.
    /// \param Index : number of specific predicate.
    SpecificCondition(CondVarClass& Cond, CondVarClass::PredicateIdType Index) throw(ThreadException);

    /// Get value of the specified predicate.
    /// \return Predicate value.
    /// \throw ThreadException
    bool getPredicate() throw(ThreadException)
    {
        return m_CondVarCl.getPredicate(m_Index);
    }
    /// Set value of the specified predicate.
    /// \param PredicateValue : predicate value.
    /// \throw ThreadException
    void setPredicate(bool PredicateValue) throw(ThreadException)
    {
        m_CondVarCl.setPredicate(m_Index, PredicateValue);
    }
    /// Signal condition variable .
    void signal()
    {
        m_CondVarCl.signal();
    }
    /// Broadcast condition variable.
    /// \throw ThreadException
    void broadcast()
    {
        m_CondVarCl.broadcast();
    }
    /// Wait for condition variable to be signaled.
    /// \return Predicates after wait.
    uint32_t wait(std::chrono::steady_clock::duration Timeout = std::chrono::steady_clock::duration::zero())
    {
        return m_CondVarCl.wait(Timeout);
    }
    /// Get internal mutex.
    /// \return reference to internal mutex.
    std::mutex& getMutex()
    {
    	return m_CondVarCl.getMutex();
    }

protected:
    CondVarClass& m_CondVarCl; ///< Condition variable object reference.
    CondVarClass::PredicateIdType m_Index; ///< Specified predicate number.
};
} // end of ThreadClassLib namespace

#endif // _CONVARCLASS_H

