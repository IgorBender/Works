/*
 * SemaphoreClass.h
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
#ifndef SEMAPHORECLASS_H
#define SEMAPHORECLASS_H

#include <semaphore.h>
#include <time.h>
#include <stdint.h>

//#include "ThreadException.h"
#include "PThreadClassLib.h"

///
/// \class SemaphoreClass
/// Wrapping on unnamed simple POSIX semaphore
class PTHREADCLASSLIB_API SemaphoreClass
{
public:
    /// Constructor
    SemaphoreClass() {}
    /// Destructor
    virtual ~SemaphoreClass() {}
#ifndef _WITHOUT_THREAD_EXCEPTIONS
    ///
    /// \details init POIX semaphore
    /// \param Count - initial value (default 0)
    /// \throw ThreadException
    void init(uint32_t Count = 0) noexcept(false);

    ///
    /// \details post semaphore
    /// \throw ThreadException
    void post() noexcept(false);

    ///
    /// \details wait for semaphore until counter is up
    /// \param TimeoutMs - wait time in milliseconds, if 0 - wait forever
    /// \return 0 - semaphore is obtained, ETIMEDOUT - timeout is expired
    /// \throw ThreadException
    int32_t wait(uint32_t TimeoutMs = 0) noexcept(false);

    ///
    /// \details wait for semaphore or exit immediatelly
    /// \return 0 - semaphore is obtained, EAGAIN - if semaphore counter equals 0
    /// \throw ThreadException
    int32_t trywait() noexcept(false);

    ///
    /// \details get semaphore current value
    /// \return - current semaphore value
    /// \throw ThreadException
    int32_t getvalue() noexcept(false);

#else
    bool init(uint32_t Count = 0);
    int32_t post();
    int32_t wait(uint32_t TimeoutMs = 0);
    int32_t trywait();
    int32_t getvalue();
#endif


protected:
    sem_t m_Semaphore; ///< POSIX semaphore
    bool m_Initialized = false;
};

#endif // SEMAPHORECLASS_H
