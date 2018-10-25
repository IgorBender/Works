/* PMutexClass.h
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

#ifndef _PMUTEXCLASS_H
#define _PMUTEXCLASS_H

#include <pthread.h>
#include <errno.h>

#include <ThreadException.h>

#ifdef WIN32
#pragma warning ( disable : 4290 )
#else
#include <unistd.h>
#endif

///
/// \class PMutexClass
/// Wrapper on POSIX mutual exclusive lock.
class PTHREADCLASSLIB_API PMutexClass
{
public:
#ifndef _WITHOUT_THREAD_EXCEPTIONS
    /// Constructor.
    /// \throw ThreadException
    PMutexClass() noexcept(false);
#else
    PMutexClass();

    bool isOk()
    {
        return Ok;
    }
#endif
    /// Destructor.
    virtual ~PMutexClass();
    
    /// Lock mutual exclusive lock.
    /// \return result of lock operation (0 - success).
    int lockMutex()
    {
        return (pthread_mutex_lock(&m_Mutex));
    }

    /// Try to lock mutual exclusive lock.
    /// \return result of lock operation (0 - success).
    int tryLockMutex()
    {
        return pthread_mutex_trylock(&m_Mutex);
    }

    /// Try to lock mutual exclusive lock with timeout.
    /// \return result of lock operation :<br/>
    /// 0 - success. <br/>
    /// ETIMEDOUT - timeout expired.<br/>
    /// other - error.
    int timedLockMutex(unsigned long Timeout = PTHREAD_INFINITE);
    
    /// Unlock mutual exclusive lock.
    /// \return result of lock operation (0 - success).
    int unlockMutex()
    {
        return pthread_mutex_unlock(&m_Mutex);
    }
    
protected:
    pthread_mutex_t m_Mutex; ///< POSIX mutual exclusive lock.
#ifdef _WITHOUT_THREAD_EXCEPTIONS
    bool Ok;
#endif
};

#endif	/* _PMUTEXCLASS_H */

