/* PBarrierClass.h
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

#ifndef _PBARRIERCLASS_H
#define _PBARRIERCLASS_H

#include <pthread.h>
#include <errno.h>

#include <ThreadException.h>

#ifdef WIN32
#pragma warning ( disable : 4290 )
#else
#include <unistd.h>
#endif

///
/// \class PBarrierClass
/// Wrapper on POSIX barrier.
class PTHREADCLASSLIB_API PBarrierClass
{
public:
#ifndef _WITHOUT_THREAD_EXCEPTIONS
    /// Constructor.
    /// \throw ThreadException
    PBarrierClass(int Counter) throw(ThreadException);
#else
    PBarrierClass(int Counter);

    bool isOk()
    {
        return Ok;
    }
#endif
    /// Destructor.
    virtual ~PBarrierClass();

    /// Wait for all threads at the barrier.
    /// \return result of wait operation (0 - success).
    int wait()
    {
        return pthread_barrier_wait(&m_Barrier);
    }

protected:
    pthread_barrier_t m_Barrier; ///< POSIX barrier.
    pthread_barrierattr_t m_BarrierAttribute; ///< POSIX barrier attribute.
    bool Ok; ///< Health status.
};

#endif
