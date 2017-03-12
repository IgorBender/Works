/* BoundSocketV6.cpp
 *
 * Copyright 2000 Igor Bender
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <BoundSocketV6.h>

#ifndef _NO_IPV6
BoundSocketV6::BoundSocketV6()
{
    memset(&m_EndPoint, 0, sizeof m_EndPoint);
    m_EndPoint.sin6_family = NO_DOMAIN;
    m_Bound = false;
}

void BoundSocketV6::setEndPoint(in6_addr Address, short Port)
{
    m_EndPoint.sin6_family = INET6_DOMAIN;
    memcpy(&m_EndPoint.sin6_addr, &Address, sizeof(in6_addr));
    m_EndPoint.sin6_port = Port;
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void BoundSocketV6::setEndPoint(const char* Address, short Port) throw(SockException)
#else
int BoundSocketV6::setEndPoint(const char* Address, short Port)
#endif
{
#ifndef _NO_IPV6
	in6_addr AddressStruct;
	int ConvertResult = inet_pton(INET6_DOMAIN, Address, &AddressStruct);
    if(0 >= ConvertResult)
    {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        SOCK_EXCEPT_THROW("Wrong address");
#else
        return SOCKET_ERROR;
#endif
    }
    setEndPoint(AddressStruct, Port);
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
void BoundSocketV6::bind() throw(SockException)
#else
int BoundSocketV6::bind()
#endif
{
#ifndef _WITHOUT_SOCK_EXCEPTIONS

    if(m_EndPoint.sin6_family != INET6_DOMAIN)
    {
        SOCK_EXCEPT_THROW("Wrong socket domain");
    }
#endif
    if(::bind(m_Sock, (struct sockaddr*)&m_EndPoint, sizeof m_EndPoint) == SOCKET_ERROR)
    {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        SOCK_EXCEPT_THROW(WSAGetLastError());
#else
        return SOCKET_ERROR;
#endif
    }
    m_Bound = true;
#ifdef _WITHOUT_SOCK_EXCEPTIONS
    return 0;
#endif
}

#endif /* _NO_IPV6 */
