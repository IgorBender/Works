/* StreamBoundV6.h
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

#ifndef STREAMBOUNDV6_H_
#define STREAMBOUNDV6_H_

#ifndef _NO_IPV6

#include <StreamSocketV6.h>
#include <BoundSocketV6.h>

/// TCP socket bound to specified port and address.
class SOCKLIB_API StreamBoundV6 : public StreamSocketV6, public BoundSocketV6
{
public:
    /// Constructor.
    /// \param Port : port number for bind in network byte order.
    /// \param Address : IPv6 address for bind.
    /// \param LateBind : do bind now or later according to bind method call.
    /// \throw SockException.
    StreamBoundV6(short Port, in6_addr Address = in6addr_any, bool LateBind = false);
    /// Constructor.
    /// \param Port : port number for bind in network byte order.
    /// \param Address : IPv6 address for bind in textual notation.
    /// \param LateBind : do bind now or later according to bind method call.
    /// \throw SockException.
    StreamBoundV6(short Port, const char* Address, bool LateBind = false);

private:
    /// Copy constructor.
    /// Makes the class uncopyable.
    StreamBoundV6(StreamBoundV6& s);
    /// Operator assign.
    /// Makes the class uncopyable.
    StreamBoundV6& operator=(StreamBoundV6& s);
};

#endif /* _NO_IPV6 */
#endif /* STREAMBOUNDV6_H_ */
