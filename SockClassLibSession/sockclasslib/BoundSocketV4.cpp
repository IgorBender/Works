/* BoundSocketV4.cpp
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

#include <BoundSocketV4.h>

BoundSocketV4::BoundSocketV4() : InternetSocketV4()
{
    memset(&m_EndPoint, 0, sizeof m_EndPoint);
    m_EndPoint.sin_family = NO_DOMAIN;
    m_Bound = false;
}

void BoundSocketV4::setEndPoint(in_addr_t Address, short Port)
{
    m_EndPoint.sin_family = INTERNET_DOMAIN;
    m_EndPoint.sin_addr.s_addr = Address;
    m_EndPoint.sin_port = Port;
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void BoundSocketV4::setEndPoint(const char* Address, short Port)
#else
int BoundSocketV4::setEndPoint(const char* Address, short Port)
#endif
{
    in_addr_t Addr = inet_addr(const_cast < char* > (Address));
    if(Addr == INADDR_NONE)
    {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        SOCK_EXCEPT_THROW("Wrong address");
#else
        return SOCKET_ERROR;
#endif
    }
    setEndPoint(Addr, Port);
#ifdef _WITHOUT_SOCK_EXCEPTIONS
    return 0;
#endif
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void BoundSocketV4::bind()
#else
int BoundSocketV4::bind()
#endif
{
#ifndef _WITHOUT_SOCK_EXCEPTIONS

    if(m_EndPoint.sin_family != INTERNET_DOMAIN)
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

