/* ServerReuseIpDual.cpp
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

#include <ServerReuseIpDual.h>

ServerReuseIpDual::ServerReuseIpDual(uint16_t Port, in6_addr Address) :
    ServerSimpleIpDual(Port, Address, true)
{
    struct linger Linger;
    Linger.l_onoff = 1;
    Linger.l_linger = 0;
    setSockLevelOpt(SO_LINGER, reinterpret_cast < const char* > (&Linger),
                    sizeof(struct linger));
    int Opt = 1;
    setSockLevelOpt(SO_REUSEADDR, reinterpret_cast < const char* > (&Opt),
                    sizeof Opt);

    if(!isBound())
    {
        setEndPoint(Address, Port);
        bind();
    }
}

ServerReuseIpDual::ServerReuseIpDual(uint16_t Port, const char* Address) :
    ServerSimpleIpDual(Port, Address, true)
{
    struct linger Linger;
    Linger.l_onoff = 1;
    Linger.l_linger = 0;
    setSockLevelOpt(SO_LINGER, reinterpret_cast < const char* > (&Linger),
                    sizeof(struct linger));
    int Opt = 1;
    setSockLevelOpt(SO_REUSEADDR, reinterpret_cast < const char* > (&Opt),
                    sizeof Opt);

    if(!isBound())
    {
        setEndPoint(Address, Port);
        bind();
    }
}

#endif /* _NO_IPV6 */
