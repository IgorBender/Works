/* ConnectedSocketV4.cpp
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

#include <ConnectedSocketV4.h>

void ConnectedSocketV4::setDestination(in_addr_t Address, short Port)
{
    m_Destination.sin_family = INTERNET_DOMAIN;
    m_Destination.sin_addr.s_addr = Address;
    m_Destination.sin_port = Port;
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void ConnectedSocketV4::setDestination(const char* Address, short Port) throw(SockException)
#else
in_addr_t ConnectedSocketV4::setDestination(const char* Address, short Port)
#endif
{
    in_addr_t  Addr = inet_addr(const_cast < char* > (Address));
    if(Addr == INADDR_NONE)
    {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        SOCK_EXCEPT_THROW("Wrong address");
#else
        return SOCKET_ERROR;
#endif

    }
    setDestination(Addr, Port);
#ifdef _WITHOUT_SOCK_EXCEPTIONS
    return 0;
#endif
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void ConnectedSocketV4::connect() throw(SockException)
{
    if(m_Connected)
        return;
    if(m_Destination.sin_family == NO_DOMAIN)
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
int ConnectedSocketV4::connect()
{
    if(m_Connected)
        return 0;
    if(m_Destination.sin_family == NO_DOMAIN)
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

