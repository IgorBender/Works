/* PRWLockClass.h
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

#ifndef PRWLOCKCLASS_H
#define	PRWLOCKCLASS_H

#include <pthread.h>
#include <errno.h>

#include "ThreadException.h"

#ifdef WIN32
#pragma warning ( disable : 4290 )
#else
#include <unistd.h>
#endif

///
/// \class PRWLockClass
/// Wrapper on POSIX read-write lock.
class PTHREADCLASSLIB_API PRWLockClass
{
public:
#ifndef _WITHOUT_THREAD_EXCEPTIONS
    /// Constructor.
    /// \throw ThreadException
    PRWLockClass() noexcept(false);
#else
    PRWLockClass();

    bool isOk()
    {
        return Ok;
    }
#endif
    /// Destructor
    virtual ~PRWLockClass();
    
    /// Lock for read.
    /// \return result of lock operation.
    int lockRead()
    {
        return (pthread_rwlock_rdlock(&m_RWLock));
    }

    /// Try to lock for read.
    /// \return result of lock operation.
    int tryLockRead()
    {
        return pthread_rwlock_tryrdlock(&m_RWLock);
    }

    /// Try to lock for read with timeout.
    /// \return result of lock operation :<br/>
    /// 0 - success. <br/>
    /// ETIMEDOUT - timeout expired.<br/>
    /// other - error.
    int timedLockRead(unsigned long Timeout = PTHREAD_INFINITE);
    
    /// Lock for write.
    /// \return result of lock operation.
    int lockWrite()
    {
        return (pthread_rwlock_wrlock(&m_RWLock));
    }

    /// Try to lock for write.
    /// \return result of lock operation.
    int tryLockWrite()
    {
        return pthread_rwlock_trywrlock(&m_RWLock);
    }

    /// Try to lock for write with timeout.
    /// \return result of lock operation :<br/>
    /// 0 - success. <br/>
    /// ETIMEDOUT - timeout expired.<br/>
    /// other - error.
    int timedLockWrite(unsigned long Timeout = PTHREAD_INFINITE);
    
    /// Unlock .
    /// \return result of lock operation (0 - success).
    int unLock()
    {
        return pthread_rwlock_unlock(&m_RWLock);
    }
    
protected:
    pthread_rwlock_t m_RWLock; ///< POSIX read-write lock;
#ifdef _WITHOUT_THREAD_EXCEPTIONS
    bool Ok;
#endif
};

#endif	/* PRWLOCKCLASS_H */

