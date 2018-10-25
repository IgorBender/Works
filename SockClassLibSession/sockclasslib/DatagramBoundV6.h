/* DatagramBoundV6.h
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

#ifndef _NO_IPV6

#ifndef DATAGRAMBOUNDV6_H_
#define DATAGRAMBOUNDV6_H_

#include <DatagramSocketV6.h>
#include <BoundSocketV6.h>

/// UDP socket bound to specified port and address (aka UDP server) for IPv6.
class SOCKLIB_API DatagramBoundV6 : public DatagramSocketV6, public BoundSocketV6
{
public:
    /// Constructor.
    /// \param Port : port number for bind in network byte order.
    /// \param Address : IPv6 address for bind in network byte order.
    /// \throw SockException.
    DatagramBoundV6(short Port, in6_addr Address = in6addr_any);
    /// Constructor.
    /// \param Port : port number for bind in network byte order.
    /// \param Address : IPv6 address for bind in textual notation.
    /// \throw SockException.
    DatagramBoundV6(short Port, const char* Address);

private:
    /// Copy constructor.
    /// Makes the class uncopyable.
    DatagramBoundV6(DatagramBoundV6& d);
    /// Operator assign.
    /// Makes the class uncopyable.
    DatagramBoundV6& operator=(DatagramBoundV6& d);
};

#endif /* _NO_IPV6 */
#endif /* DATAGRAMBOUNDV6_H_ */
