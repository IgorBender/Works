/* StreamBound.h
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

#ifndef _STREAMBOUNDV4_H
#define _STREAMBOUNDV4_H

#include <StreamSocketV4.h>
#include <BoundSocketV4.h>

/// TCP socket bound to specified port and address.
class SOCKLIB_API StreamBoundV4 : public StreamSocketV4, public BoundSocketV4
{
public:
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Constructor.
    /// \param Port : port number for bind in network byte order.
    /// \param Address : IPv4 address for bind in network byte order.
    /// \param LateBind : do bind now or later according to bind method call.
    /// \throw SockException.
    StreamBoundV4(short Port, in_addr_t Address = INADDR_ANY, bool LateBind = false) throw (SockException);
    /// Constructor.
    /// \param Port : port number for bind in network byte order.
    /// \param Address : IPv4 address for bind in decimal dot notation.
    /// \param LateBind : do bind now or later according to bind method call.
    /// \throw SockException.
    StreamBoundV4(short Port, const char* Address, bool LateBind = false) throw (SockException);
#else
    StreamBoundV4(short Port, in_addr_t Address = INADDR_ANY,
                bool LateBind = false);
    StreamBoundV4(short Port, const char* Address, bool LateBind = false);
#endif

private:
    /// Copy constructor.
    /// Makes the class uncopyable.
    StreamBoundV4(StreamBoundV4& s);
    /// Operator assign.
    /// Makes the class uncopyable.
    StreamBoundV4& operator=(StreamBoundV4& s);
};

#endif /* _STREAMBOUNDV4_H */

