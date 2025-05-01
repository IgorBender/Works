/* StreamSocketUds.cpp
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

#include "StreamSocketUds.h"

StreamSocketUds::StreamSocketUds()
{
    m_GotSockName = false;
    m_GotPeerName = false;
    memset(&m_SockName, 0, sizeof m_SockName);
    memset(&m_PeerName, 0, sizeof m_PeerName);
}

StreamSocketUds::StreamSocketUds(SOCKET sock)
{
    m_Domain = UNIX_DOMAIN;
    m_Type = TCP_SOCK;
    m_Protocol = 0;
    m_Sock = sock;
    m_GotSockName = false;
    m_GotPeerName = false;
}

StreamSocketUds::~StreamSocketUds()
{
    close();
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
sockaddr_un& StreamSocketUds::getSockName()
{
    if(!m_GotSockName)
    {
        socklen_type Len = sizeof m_SockName;
        if(getsockname(m_Sock, reinterpret_cast <sockaddr*> (&m_SockName),
                       &Len) == SOCKET_ERROR)
        {
            SOCK_EXCEPT_THROW(WSAGetLastError(), m_Sock);
        }
        m_GotSockName = true;
    }
    return m_SockName;
}

sockaddr_un& StreamSocketUds::getPeerName()
{
    if(!m_GotPeerName)
    {
        socklen_type Len = sizeof m_PeerName;
        if(getpeername(m_Sock, reinterpret_cast <sockaddr*> (&m_PeerName),
                       &Len) == SOCKET_ERROR)
        {
            SOCK_EXCEPT_THROW(WSAGetLastError(), m_Sock);
        }
        m_GotPeerName = true;
    }
    return m_PeerName;
}
#else // _WITHOUT_SOCK_EXCEPTIONS
int StreamSocketUds::getSockName(sockaddr_un* SockAddr)
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

int StreamSocketUds::getPeerName(sockaddr_un* PeerAddr)
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

