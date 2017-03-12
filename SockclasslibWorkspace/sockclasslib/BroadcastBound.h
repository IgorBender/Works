/* BroadcastBound.h
 *
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

#ifndef _BROADCASTBOUND_H
#define _BROADCASTBOUND_H

#include <BroadcastSocket.h>
#include <BoundSocketV4.h>

/// Bounded socket with broadcast capabilities.
class SOCKLIB_API BroadcastBound : public BroadcastSocket, public BoundSocketV4
{
public:
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Constructor.
    /// \param Port : port number for bind in network byte order.
    /// \param Address : address for bind in network byte order.
    /// \throw SockException.
    BroadcastBound(short Port, in_addr_t Address = INADDR_ANY) throw(SockException);
    /// Constructor.
    /// \param Port : port number for bind in network byte order.
    /// \param Address : address for bind in decimal dot notation.
    /// \throw SockException.
    BroadcastBound(short Port, const char* Address) throw(SockException);
#else
    BroadcastBound(short Port, in_addr_t Address = INADDR_ANY);
    BroadcastBound(short Port, const char* Address);
#endif

private:
    /// Copy constructor.
    /// Makes the class uncopyable.
    BroadcastBound(BroadcastBound& d);
    /// Operator assign.
    /// Makes the class uncopyable.
    BroadcastBound& operator= (BroadcastBound& d);
};

#endif
