/* MulticastBound.cpp
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

#include <MulticastBound.h>

#ifdef _MSC_VER
#include <ws2tcpip.h>
#endif

#include <errno.h>

#include <algorithm>
using namespace std;

MulticastBound::MulticastBound(uint16_t Port, in_addr_t Address) : Addr(Address)
{
    int Opt = 1;
    setSockLevelOpt(SO_REUSEADDR, reinterpret_cast < const char* > (&Opt),
        sizeof Opt);

    if(!isBound())
    {
#ifdef _MSC_VER
        setEndPoint(static_cast < in_addr_t > (Addr), Port);
#else
        setEndPoint(static_cast < in_addr_t > (INADDR_ANY), Port);
#endif
        bind();
    }
}

MulticastBound::MulticastBound(uint16_t Port, const char* Address)
{
    if(!isBound())
    {
        Addr = inet_addr(const_cast < char* > (Address));
        if(Addr == INADDR_NONE)
        {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
            SOCK_EXCEPT_THROW("Bad address", m_Sock);
#else
            return;
#endif
        }
#ifdef _MSC_VER
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
        SOCK_EXCEPT_THROW("Bad address string", m_Sock);
#endif

    }
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void MulticastBound::joinGroup(in_addr_t Group)
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
        SOCK_EXCEPT_THROW(WSAGetLastError(), m_Sock);
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
        SOCK_EXCEPT_THROW(WSAGetLastError(), m_Sock);
#else
        return SOCKET_ERROR;
#endif
    }
    GroupsList.remove(Group);
#ifdef _WITHOUT_SOCK_EXCEPTIONS
    return 0;
#endif
}

void MulticastBound::leaveAllGroups()
{
    while(!GroupsList.empty())
    {
        leaveGroup(*(GroupsList.begin()));
    }
}

