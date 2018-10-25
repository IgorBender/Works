/* ServerReuseV6.h
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

#ifndef SERVERREUSEV6_H_
#define SERVERREUSEV6_H_

#ifndef _NO_IPV6

#include <ServerSimpleV6.h>

/// TCP server socket with reuse local address ability.
class SOCKLIB_API ServerReuseV6 : public ServerSimpleV6
{
public:
    /// Constructor.
    /// \param Port : port number for bind in network byte order.
    /// \param Address : IPv6 address for bind in network byte order.
    /// \throw SockException.
    ServerReuseV6(short Port, in6_addr Address = in6addr_any);
    /// Constructor.
    /// \param Port : port number for bind in network byte order.
    /// \param Address : IPv6 address for bind in textual notation.
    /// \throw SockException.
    ServerReuseV6(short Port, const char* Address);

private:
    /// Copy constructor.
    /// Makes the class uncopyable.
    ServerReuseV6(ServerReuseV6& s);
    /// Operator assign.
    /// Makes the class uncopyable.
    ServerReuseV6& operator=(ServerReuseV6& s);
};

#endif /* _NO_IPV6 */
#endif /* SERVERREUSEV6_H_ */
