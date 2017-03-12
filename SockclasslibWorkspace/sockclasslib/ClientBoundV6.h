/* ClientBoundV6.h
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

#ifndef _CLIENTBOUNDV6_H
#define _CLIENTBOUNDV6_H

#ifndef _NO_IPV6

#include <ClientSimpleV6.h>
#include <BoundSocketV6.h>

/// Client socket bound to specified port and address.
class SOCKLIB_API ClientBoundV6 : public ClientSimpleV6, public BoundSocketV6
{
public:
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Constructor.
    /// \param Port : port number for bind in network byte order.
    /// \param Address : IPv6 address for bind in network byte order.
    /// \param LateBind : do bind now or later according to bind method call.
    /// \throw SockException.
    ClientBoundV6(short Port, in6_addr Address = in6addr_any, bool LateBind = false) throw(SockException)
    {
        if(!isBound())
        {
            setEndPoint(Address, Port);
            if(!LateBind)
            {
                bind();
            }
        }
    }

    /// Constructor.
    /// \param Port : port number for bind in network byte order.
    /// \param Address : IPv6 address for bind in textual notation.
    /// \param LateBind : do bind now or later according to bind method call.
    /// \throw SockException.
    ClientBoundV6(short Port, const char* Address, bool LateBind = false) throw(SockException)
    {
        if(!isBound())
        {
            setEndPoint(Address, Port);
            if(!LateBind)
            {
                bind();
            }
        }
    }
#else
    ClientBoundV6(short Port, in6_addr Address = in6addr_any, bool LateBind = false)
    {
        if(!isBound())
        {
            setEndPoint(Address, Port);
            if(!LateBind)
            {
                bind();
            }
        }
    }

    ClientBoundV6(short Port, const char* Address, bool LateBind = false)
    {
        if(!isBound())
        {
            setEndPoint(Address, Port);
            if(!LateBind)
            {
                bind();
            }
        }
    }
#endif

private:
    /// Copy constructor.
    /// Makes the class uncopyable.
    ClientBoundV6(ClientBoundV6& s);
    /// Operator assign.
    /// Makes the class uncopyable.
    ClientBoundV6& operator= (ClientBoundV6& s);
};

#endif /* _NO_IPV6 */
#endif /* _CLIENTBOUNDV6_H */

