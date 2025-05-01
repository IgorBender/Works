/* PBarrierClass.cpp
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

#include "PBarrierClass.h"


#ifndef _WITHOUT_THREAD_EXCEPTIONS
PBarrierClass::PBarrierClass(uint32_t Counter) noexcept(false) : Ok(true)
#else
PBarrierClass::PBarrierClass(int Counter) : Ok(true)
#endif
{
    int Result = pthread_barrierattr_init(&m_BarrierAttribute);
    if(0 != Result)
    {
        Ok = false;
#ifndef _WITHOUT_THREAD_EXCEPTIONS
        THREAD_EXCEPT_THROW(Result);
#else
        return;
#endif
    }
    Result = pthread_barrier_init(&m_Barrier, &m_BarrierAttribute, Counter);
    if(0 != Result)
    {
        pthread_barrierattr_destroy(&m_BarrierAttribute);
        Ok = false;
#ifndef _WITHOUT_THREAD_EXCEPTIONS
        THREAD_EXCEPT_THROW(Result);
#endif
    }
    pthread_barrierattr_destroy(&m_BarrierAttribute);
}

PBarrierClass::~PBarrierClass()
{
    if(Ok)
    {
        pthread_barrier_destroy(&m_Barrier);
    }
}
