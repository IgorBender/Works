/* MulticastBoundV6.cpp
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

#include "MulticastBoundV6.h"

#ifndef _NO_IPV6

#ifdef _WIN32
#include <ws2tcpip.h>
#endif

#include <errno.h>

#include <algorithm>
using namespace std;

#ifndef _WITHOUT_SOCK_EXCEPTIONS
MulticastBoundV6::MulticastBoundV6(short Port, in6_addr Address)
    throw(SockException) : Addr(Address)
#else
MulticastBoundV6::MulticastBoundV6(short Port, in6_addr Address) : Addr(Address)
#endif
{
    int Opt = 1;
    setSockLevelOpt(SO_REUSEADDR, reinterpret_cast < const char* > (&Opt),
        sizeof Opt);

    if(!isBound())
    {
#ifdef _WIN32
        setEndPoint(static_cast < in6_addr > (Addr), Port);
#else
        setEndPoint(in6addr_any, Port);
#endif
        bind();
    }
}

MulticastBoundV6::MulticastBoundV6(short Port, const char* Address)
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    throw(SockException)
#endif
{
    if(!isBound())
    {
#ifdef _WIN32
        setEndPoint(static_cast < in6_addr > (Addr), Port);
#else
        setEndPoint(in6addr_any, Port);
#endif
        bind();
    }
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void MulticastBoundV6::joinGroup(in6_addr Group)
    throw(SockException)
#else
int MulticastBoundV6::joinGroup(in6_addr Group)
#endif
{
    if(find(GroupsList.begin(), GroupsList.end(), Group) !=
            GroupsList.end())
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        return;
#else
    return SOCKET_ERROR;
#endif

    ipv6_mreq rReq;
    rReq.ipv6mr_multiaddr = Group;
    rReq.ipv6mr_interface = m_Index;
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    setIpLevelOpt(IPV6_ADD_MEMBERSHIP,
#else
    int iResult = setIpLevelOpt(IPV6_ADD_MEMBERSHIP,
#endif
#ifndef __VXWORKS__
                             reinterpret_cast < const char* > (&rReq),
#else
                             reinterpret_cast < char* > (&rReq),
#endif
                             sizeof(rReq));
#ifdef _WITHOUT_SOCK_EXCEPTIONS
	if (0 > iResult)
	{
		return iResult;
	}
#endif

    GroupsList.push_back(Group);
#ifdef _WITHOUT_SOCK_EXCEPTIONS
    return iResult;
#endif
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void MulticastBoundV6::leaveGroup(in6_addr Group)
    throw(SockException)
#else
int MulticastBoundV6::leaveGroup(in6_addr Group)
#endif
{
    if(find(GroupsList.begin(), GroupsList.end(), Group) ==
            GroupsList.end())
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        return;
#else
    return SOCKET_ERROR;
#endif
    ipv6_mreq rReq;
    rReq.ipv6mr_multiaddr = Group;
    rReq.ipv6mr_interface = m_Index;
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    setIpLevelOpt(IPV6_DROP_MEMBERSHIP,
#else
    int iResult = setIpLevelOpt(IPV6_DROP_MEMBERSHIP,
#endif
#ifndef __VXWORKS__
                             reinterpret_cast < const char* > (&rReq),
#else
                             reinterpret_cast < char* > (&rReq),
#endif
                             sizeof(rReq));
#ifdef _WITHOUT_SOCK_EXCEPTIONS
    return iResult;
#endif
    GroupsList.remove(Group);
#ifdef _WITHOUT_SOCK_EXCEPTIONS
    return 0;
#endif
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void MulticastBoundV6::leaveAllGroups() throw(SockException)
#else
void MulticastBoundV6::leaveAllGroups()
#endif
{
    while(!GroupsList.empty())
    {
        leaveGroup(*(GroupsList.begin()));
    }
}

#endif /* _NO_IPV6 */
