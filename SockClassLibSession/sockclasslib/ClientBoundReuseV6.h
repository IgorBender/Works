/* ClientBoundReuseV6.h
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

#ifndef CLIENTBOUNDREUSEV6_H
#define CLIENTBOUNDREUSEV6_H

#include "ClientSimpleV6.h"
#include "BoundSocketV6.h"

/// Client socket bound to specified port and address with socket reuse.
class ClientBoundReuseV6 : public ClientSimpleV6, public BoundSocketV6
{
public:
    /// Constructor.
    /// \param Port : port number for bind in network byte order.
    /// \param Address : IPv6 address for bind in network byte order.
    /// \param LateBind : do bind now or later according to bind method call.
    /// \throw SockException.
    ClientBoundReuseV6(uint16_t Port, in6_addr Address = in6addr_any,
                       bool LateBind = false);

    /// Constructor.
    /// \param Port : port number for bind in network byte order.
    /// \param Address : IPv6 address for bind in textual notation.
    /// \param LateBind : do bind now or later according to bind method call.
    /// \throw SockException.
    ClientBoundReuseV6(uint16_t Port, const char* Address,
                       bool LateBind = false);

private:
    /// Copy constructor.
    /// Makes the class uncopyable.
    ClientBoundReuseV6(ClientBoundReuseV6& s);
    /// Operator assign.
    /// Makes the class uncopyable.
    ClientBoundReuseV6& operator= (ClientBoundReuseV6& s);
};

#endif // CLIENTBOUNDREUSEV6_H
