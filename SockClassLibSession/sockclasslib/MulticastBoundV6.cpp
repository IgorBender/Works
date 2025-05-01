/* MulticastBoundV6.cpp
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

#include "MulticastBoundV6.h"

#ifndef _NO_IPV6

#ifdef _MSC_VER
#include <ws2tcpip.h>
#endif

#include <errno.h>

#include <algorithm>
using namespace std;

MulticastBoundV6::MulticastBoundV6(uint16_t Port, in6_addr Address) : Addr(Address)
{
    int Opt = 1;
    setSockLevelOpt(SO_REUSEADDR, reinterpret_cast < const char* > (&Opt),
        sizeof Opt);

    if(!isBound())
    {
#ifdef _MSC_VER
        setEndPoint(static_cast < in6_addr > (Addr), Port);
#else
        setEndPoint(in6addr_any, Port);
#endif
        bind();
    }
}

MulticastBoundV6::MulticastBoundV6(uint16_t Port, const char* Address)
{
    if(!isBound())
    {
#ifdef _MSC_VER
        setEndPoint(Address, Port);
#else
        inet_pton(AF_INET6, Address, &Addr);
        setEndPoint(in6addr_any, Port);
#endif
        bind();
    }
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void MulticastBoundV6::joinGroup(in6_addr Group)
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
    if(0 != iResult)
        return iResult;
#endif
    GroupsList.remove(Group);
#ifdef _WITHOUT_SOCK_EXCEPTIONS
    return 0;
#endif
}

void MulticastBoundV6::leaveAllGroups()
{
    while(!GroupsList.empty())
    {
        leaveGroup(*(GroupsList.begin()));
    }
}

#endif /* _NO_IPV6 */
