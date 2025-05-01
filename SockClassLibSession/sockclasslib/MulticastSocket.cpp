/* MulticastSocket.cpp
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

#include <MulticastSocket.h>

#ifdef _MSC_VER
#include <ws2tcpip.h>
#endif

MulticastSocket::~MulticastSocket()
{}

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
        SOCK_EXCEPT_THROW("Wrong address", m_Sock);
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

