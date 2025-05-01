/*
 *  CondVarCountedClass.h
 *
 *  Created on: 2021-12-03
 *  Author: Igor Bender
 *
 */
#ifndef CONDVARCOUNTEDCLASS_H
#define CONDVARCOUNTEDCLASS_H

#include <errno.h>
#include <cstdint>
#include <condition_variable>
#include <mutex>

#include "ThreadException.h"

namespace ThreadClassLib
{
///
/// \class CondVarClass
/// Wrapping on C++ condition variable.
class CondVarCountedClass
{
public:
    /// Constructor.
    /// \param PredNum : number of predicates (up to 32).
    CondVarCountedClass() {}
    /// Destructor
    virtual ~CondVarCountedClass() {}

    /// Get value of the internal counter, should be protected by mutex lock.
    /// \return Counter value.
    int32_t getConter()
    {
        return m_Counter;
    }

    /// Get internal mutex.
    /// \return reference to internal mutex.
    std::mutex& getMutex()
    {
        return m_Mutex;
    }

    /// Signal condition variable .
    void signal();

    /// Wait for condition variable to be signaled.
    /// \returns = wait result.
    bool wait(std::chrono::steady_clock::duration Timeout = std::chrono::steady_clock::duration::zero());

protected:
    std::condition_variable m_CondVar; ///< C++ condition variable.
    std::mutex m_Mutex; ///< Associated mutual exclusive lock.
    int32_t m_Counter = 0; ///< Condition variable signal counter.
};

} // end of ThreadClassLib namespace

#endif // CONDVARCOUNTEDCLASS_H
