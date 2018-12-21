/* ClientSimplev6.h
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

#ifndef CLIENTSIMPLEV6_H
#define CLIENTSIMPLEV6_H

#ifndef _NO_IPV6

#include <StreamSocketV6.h>
#include <ConnectedSocketV6.h>

/// Stream oriented socket with connection ability for IPv6.
class ClientSimpleV6 : public StreamSocketV6, public ConnectedSocketV6
{
public:
    /// Constructor.
    /// Default constructor.
    ClientSimpleV6()
    {
    }

private:
    /// Copy constructor.
    /// Makes the class uncopyable.
    ClientSimpleV6(ClientSimpleV6& s);
    /// Operator assign.
    /// Makes the class uncopyable.
    ClientSimpleV6& operator=(ClientSimpleV6& s);
};

#endif /* _NO_IPV6 */
#endif /* CLIENTSIMPLEV6_H */

