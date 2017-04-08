/* StreamSocketV6.cpp
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

#ifndef _NO_IPV6

#include "StreamSocketV6.h"

#ifndef _WITHOUT_SOCK_EXCEPTIONS
StreamSocketV6::StreamSocketV6() throw(SockException)
#else
StreamSocketV6::StreamSocketV6()
#endif
{
#ifndef _NO_IPV6
	m_Domain = INET6_DOMAIN;
    m_Type = TCP_SOCK;
    m_Protocol = 0;
    memset(&m_SockName, 0, sizeof m_SockName);
    memset(&m_PeerName, 0, sizeof m_PeerName);
#else
    m_Domain = NO_DOMAIN;

#ifndef _WITHOUT_SOCK_EXCEPTIONS
    SOCK_EXCEPT_THROW("No IPv6 support defined");
#else
    return;
#endif
#endif
}

StreamSocketV6::StreamSocketV6(SOCKET sock)
{
	m_Domain = INET6_DOMAIN;
    m_Type = TCP_SOCK;
    m_Protocol = 0;
    m_Sock = sock;
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
sockaddr_in6& StreamSocketV6::getSockName() throw(SockException)
{
    if(!m_GotSockName)
    {
        socklen_type Len = sizeof m_SockName;
        if(getsockname(m_Sock, reinterpret_cast <sockaddr*> (&m_SockName),
                       &Len) == SOCKET_ERROR)
        {
            SOCK_EXCEPT_THROW(WSAGetLastError());
        }
        m_GotSockName = true;
    }
    return m_SockName;
}

sockaddr_in6& StreamSocketV6::getPeerName() throw(SockException)
{
    if(!m_GotPeerName)
    {
        socklen_type Len = sizeof m_PeerName;
        if(getpeername(m_Sock, reinterpret_cast <sockaddr*> (&m_PeerName),
                       &Len) == SOCKET_ERROR)
        {
            SOCK_EXCEPT_THROW(WSAGetLastError());
        }
        m_GotPeerName = true;
    }
    return m_PeerName;
}
#else // _WITHOUT_SOCK_EXCEPTIONS
int StreamSocketV6::getSockName(sockaddr_in6* SockAddr)
{
    if(!m_GotSockName)
    {
        socklen_type Len = sizeof m_SockName;
        if(getsockname(m_Sock, reinterpret_cast <sockaddr*> (&m_SockName),
                       &Len) == SOCKET_ERROR)
        {
            return SOCKET_ERROR;
        }
        m_GotSockName = true;
    }
    *SockAddr = m_SockName;
    return 0;
}

int StreamSocketV6::getPeerName(sockaddr_in6* PeerAddr)
{
    if(!m_GotPeerName)
    {
        socklen_type Len = sizeof m_PeerName;
        if(getpeername(m_Sock, reinterpret_cast <sockaddr*> (&m_PeerName),
                       &Len) == SOCKET_ERROR)
        {
            return SOCKET_ERROR;
        }
        m_GotPeerName = true;
    }
    *PeerAddr = m_PeerName;
    return 0;
}
#endif // _WITHOUT_SOCK_EXCEPTIONS

#endif /* _NP_IPV6 */
