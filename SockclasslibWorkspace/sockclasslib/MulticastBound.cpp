/* MulticastBound.cpp
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

#include <MulticastBound.h>

#ifdef _WIN32
#include <ws2tcpip.h>
#endif

#include <errno.h>

#include <algorithm>
using namespace std;

#ifndef _WITHOUT_SOCK_EXCEPTIONS
MulticastBound::MulticastBound(short Port, in_addr_t Address)
    throw(SockException) : Addr(Address)
#else
MulticastBound::MulticastBound(short Port, in_addr_t Address) : Addr(Address)
#endif
{
    int Opt = 1;
    setSockLevelOpt(SO_REUSEADDR, reinterpret_cast < const char* > (&Opt),
        sizeof Opt);

    if(!isBound())
    {
#ifdef _WIN32
        setEndPoint(static_cast < in_addr_t > (Addr), Port);
#else
        setEndPoint(static_cast < in_addr_t > (INADDR_ANY), Port);
#endif
        bind();
    }
}

MulticastBound::MulticastBound(short Port, const char* Address)
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    throw(SockException)
#endif
{
    if(!isBound())
    {
        Addr = inet_addr(const_cast < char* > (Address));
        if(Addr == INADDR_NONE)
        {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
            SOCK_EXCEPT_THROW("Bad address");
#else
            return;
#endif
        }
#ifdef _WIN32
        setEndPoint(static_cast < in_addr_t > (Addr), Port);
#else
        setEndPoint(static_cast < in_addr_t > (INADDR_ANY), Port);
#endif
        bind();
    }
    in_addr rAddr;
    rAddr.s_addr = inet_addr(const_cast < char* > (Address));
    if(rAddr.s_addr == INADDR_NONE)
    {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        SOCK_EXCEPT_THROW("Bad address string");
#endif

    }
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void MulticastBound::joinGroup(in_addr_t Group)
    throw(SockException)
#else
int MulticastBound::joinGroup(in_addr_t Group)
#endif
{
    if(find(GroupsList.begin(), GroupsList.end(), Group) !=
            GroupsList.end())
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        return;
#else
    return SOCKET_ERROR;
#endif

    ip_mreq rReq;
    rReq.imr_multiaddr.s_addr = Group;
    rReq.imr_interface.s_addr = Addr;
    int iResult = setsockopt(m_Sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
#ifndef __VXWORKS__
                             reinterpret_cast < const char* > (&rReq),
#else
                             reinterpret_cast < char* > (&rReq),
#endif
                             sizeof(rReq));
    if(iResult == SOCKET_ERROR)
    {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        SOCK_EXCEPT_THROW(WSAGetLastError());
#else
        return SOCKET_ERROR;
#endif

    }
    GroupsList.push_back(Group);
#ifdef _WITHOUT_SOCK_EXCEPTIONS
    return 0;
#endif
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void MulticastBound::leaveGroup(in_addr_t Group)
    throw(SockException)
#else
int MulticastBound::leaveGroup(in_addr_t Group)
#endif
{
    if(find(GroupsList.begin(), GroupsList.end(), Group) ==
            GroupsList.end())
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        return;
#else
    return SOCKET_ERROR;
#endif
    ip_mreq rReq;
    rReq.imr_multiaddr.s_addr = Group;
    rReq.imr_interface.s_addr = Addr;
    int iResult = setsockopt(m_Sock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
#ifndef __VXWORKS__
                             reinterpret_cast < const char* > (&rReq),
#else
                             reinterpret_cast < char* > (&rReq),
#endif
                             sizeof(rReq));
    if(iResult == SOCKET_ERROR)
    {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        SOCK_EXCEPT_THROW(WSAGetLastError());
#else
        return SOCKET_ERROR;
#endif
    }
    GroupsList.remove(Group);
#ifdef _WITHOUT_SOCK_EXCEPTIONS
    return 0;
#endif
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void MulticastBound::leaveAllGroups() throw(SockException)
#else
void MulticastBound::leaveAllGroups()
#endif
{
    while(!GroupsList.empty())
    {
        leaveGroup(*(GroupsList.begin()));
    }
}

