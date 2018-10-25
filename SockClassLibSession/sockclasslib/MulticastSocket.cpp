/* MulticastSocket.cpp
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

#include <MulticastSocket.h>

#ifdef _WIN32
#include <ws2tcpip.h>
#endif


#ifndef _WITHOUT_SOCK_EXCEPTIONS
void MulticastSocket::setTTL(unsigned char ttl)
{
    setIpLevelOpt(IP_MULTICAST_TTL,
#ifndef __VXWORKS__
                  reinterpret_cast < const char* > (&ttl),
#else
                  reinterpret_cast < char* > (&ttl),
#endif
                  sizeof(ttl));
}
#else
int MulticastSocket::setTTL(unsigned char ttl)
{
    int Result = setIpLevelOpt(IP_MULTICAST_TTL,
#ifndef __VXWORKS__
                            reinterpret_cast < const char* > (&ttl),
#else
                            reinterpret_cast < char* > (&ttl),
#endif
                            sizeof(ttl));
    return Result;
}
#endif

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void MulticastSocket::setDefaultIf(in_addr_t Address)
{
    in_addr rAddr;
    rAddr.s_addr = Address;
    setIpLevelOpt(IP_MULTICAST_IF,
#ifndef __VXWORKS__
                  reinterpret_cast < const char* > (&rAddr),
#else
                  reinterpret_cast < char* > (&rAddr),
#endif
                  sizeof rAddr);
}
#else
int MulticastSocket::setDefaultIf(in_addr_t Address)
{
    in_addr rAddr;
    rAddr.s_addr = Address;
    int iResult = setsockopt(m_Sock, IPPROTO_IP, IP_MULTICAST_IF,
#ifndef __VXWORKS__
                             reinterpret_cast < const char* > (&rAddr),
#else
                             reinterpret_cast < char* > (&rAddr),
#endif
                             sizeof rAddr);
    return iResult;
}
#endif

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void  MulticastSocket::setDefaultIf(const char* Address)
#else
int  MulticastSocket::setDefaultIf(const char* Address)
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
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    setDefaultIf(Addr);
#else
    return setDefaultIf(Addr);
#endif
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void MulticastSocket::setLoopBack(bool On)
{
    unsigned char Loop = On ? 1 : 0;
    setIpLevelOpt(IP_MULTICAST_LOOP,
#ifndef __VXWORKS__
                  reinterpret_cast < const char* > (&Loop),
#else
                  reinterpret_cast < char* > (&Loop),
#endif
                  sizeof Loop);
}
#else
int MulticastSocket::setLoopBack(bool On)
{
    unsigned char Loop = On ? 1 : 0;
    int iResult = setsockopt(m_Sock, IPPROTO_IP, IP_MULTICAST_LOOP,
#ifndef __VXWORKS__
                             reinterpret_cast < const char* > (&Loop),
#else
                             reinterpret_cast < char* > (&Loop),
#endif
                             sizeof Loop);
    return iResult;
}
#endif

