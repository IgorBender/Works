/* ConnectedSocketV6.cpp
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

#include "ConnectedSocketV6.h"

void ConnectedSocketV6::setDestination(in6_addr Address, short Port)
{
    m_Destination.sin6_family = INET6_DOMAIN;
    memcpy(&m_Destination.sin6_addr, &Address, sizeof(in6_addr));
    m_Destination.sin6_port = Port;
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void ConnectedSocketV6::setDestination(const char* Address, short Port) throw(SockException)
#else
int ConnectedSocketV6::setDestination(const char* Address, short Port)
#endif
{
#ifndef _NO_IPV6
	in6_addr AddrStruct;
	int ConvertRes = inet_pton(INET6_DOMAIN, Address, &AddrStruct);
    if(0 >= ConvertRes)
    {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        SOCK_EXCEPT_THROW("Wrong address");
#else
        return SOCKET_ERROR;
#endif
    }
    setDestination(AddrStruct, Port);
#else
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    SOCK_EXCEPT_THROW("No IPv6 support defined");
#else
    return SOCKET_ERROR;
#endif
#endif
#ifdef _WITHOUT_SOCK_EXCEPTIONS
    return 0;
#endif
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void ConnectedSocketV6::connect() throw(SockException)
{
    if(m_Connected)
        return;
    if(m_Destination.sin6_family == NO_DOMAIN)
    {
        SOCK_EXCEPT_THROW("Wrong socket domain");
    }
    int Result;
    if((Result = ::connect(m_Sock,
                           reinterpret_cast <sockaddr*> (&m_Destination),
                           sizeof m_Destination)) == SOCKET_ERROR) {
        if(WSAEISCONN == WSAGetLastError())
        {
            m_Connected = true;
            return;
        }
        SOCK_EXCEPT_THROW(WSAGetLastError());
    }
    m_Connected = true;
}
#else // _WITHOUT_SOCK_EXCEPTIONS
int ConnectedSocketV6::connect()
{
    if(m_Connected)
        return 0;
    if(m_Destination.sin6_family == NO_DOMAIN)
    {
        return SOCKET_ERROR;
    }
    int Result;
    if((Result = ::connect(m_Sock,
                           reinterpret_cast <sockaddr*> (&m_Destination),
                           sizeof m_Destination)) == SOCKET_ERROR) {
        if(WSAEISCONN == WSAGetLastError())
        {
            m_Connected = true;
            return 0;
        }
        return SOCKET_ERROR;
    }
    m_Connected = true;
    return 0;
}
#endif // _WITHOUT_SOCK_EXCEPTIONS

#endif /* _NO_IPV6 */
