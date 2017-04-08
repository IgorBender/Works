/* DatagramSocketV6.cpp
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

#ifndef _NO_IPV6

#include <DatagramSocketV6.h>
#include <string.h>

// See article Q263823 on support.microsoft.com
#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR,12)

#ifndef _WITHOUT_SOCK_EXCEPTIONS
DatagramSocketV6::DatagramSocketV6(int protocol) throw(SockException)
#else
DatagramSocketV6::DatagramSocketV6(int protocol)
#endif
{
    m_Type = UDP_SOCK;
    m_Protocol = protocol;
    memset(&m_Source, 0, sizeof m_Source); // m_Destination is initialized in base class
    m_Source.sin6_family = NO_DOMAIN;
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    create();
#else
    if(create() == SOCKET_ERROR)
    {
        IsOk = false;
        return;
    }
#endif
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
int DatagramSocketV6::sendTo(const void* Buffer, size_t Length, int Flags) throw(SockException)
#else
int DatagramSocketV6::sendTo(const void* Buffer, size_t Length, int Flags)
#endif
{
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    if(m_Destination.sin6_family == NO_DOMAIN)
    {
        SOCK_EXCEPT_THROW("Wrong socket domain");
    }
#endif
    if(Length == 0)
        return 0;

    int Result;

    if((Result = sendto(m_Sock,
#ifndef __VXWORKS__
                        reinterpret_cast < const char* > (Buffer),
#else
                        const_cast < char* > (reinterpret_cast < const char* > (Buffer)),
#endif
                        static_cast < int > (Length), Flags,
                        reinterpret_cast < struct sockaddr* > (&m_Destination),
                        sizeof m_Destination)) == SOCKET_ERROR)

    {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        SOCK_EXCEPT_THROW(WSAGetLastError());
#endif

    }
    return Result;
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
int DatagramSocketV6::receiveFrom(void* Buffer, size_t Length, int Flags) throw(SockException)
#else
int DatagramSocketV6::receiveFrom(void* Buffer, size_t Length, int Flags)
#endif
{
    if(Length == 0)
        return 0;

    int Result;
    socklen_type Size = sizeof(struct sockaddr_in6);
    memset(&m_Source, '\0', Size);
    if((Result = recvfrom(m_Sock, reinterpret_cast < char* > (Buffer), static_cast < int > (Length), Flags,
                          reinterpret_cast <struct sockaddr*> (&m_Source),
                          &Size)) == SOCKET_ERROR)
    {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        SOCK_EXCEPT_THROW(WSAGetLastError());
#endif

    }
    return Result;
}

#endif /* _NO_IPV6 */
