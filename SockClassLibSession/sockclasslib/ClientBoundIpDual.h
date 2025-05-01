/* ClientBoundIpDual.h
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

#ifndef CLIENTBOUNDIPDUAL_H
#define CLIENTBOUNDIPDUAL_H

#ifndef _NO_IPV6

#include <ClientSimpleV6.h>
#include <BoundSocketIpDual.h>.h>

/// Client socket bound to specified port and address.
class SOCKLIB_API ClientBoundIpDual : public ClientSimpleV6,
                                      public BoundSocketIpDual
{
public:
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        /// Constructor.
    /// \param Port : port number for bind in network byte order.
    /// \param Address : IPv6 address for bind in network byte order.
    /// \param LateBind : do bind now or later according to bind method call.
    /// \throw SockException.
    ClientBoundIpDual(uint16_t Port, in6_addr Address = in6addr_any,
                      bool LateBind = false)
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
    ClientBoundIpDual(uint16_t Port, const char* Address, bool LateBind = false)
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
    ClientBoundIpDual(uint16_t Port, in6_addr Address = in6addr_any,
                      bool LateBind = false)
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

    ClientBoundIpDual(uint16_t Port, const char* Address, bool LateBind = false)
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
    ClientBoundIpDual(ClientBoundIpDual& s);
    /// Operator assign.
    /// Makes the class uncopyable.
    ClientBoundIpDual& operator= (ClientBoundIpDual& s);
};

#endif /* _NO_IPV6 */

#endif // CLIENTBOUNDIPDUAL_H
