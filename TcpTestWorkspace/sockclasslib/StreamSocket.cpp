/* StreamSocket.cpp
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

#include <StreamSocket.h>

#ifndef _WITHOUT_SOCK_EXCEPTIONS
StreamSocket::StreamSocket() throw(SockException)
#else
StreamSocket::StreamSocket()
#endif
{
    m_GotSockName = false;
    m_GotPeerName = false;
    memset(&m_SockName, 0, sizeof m_SockName);
    memset(&m_PeerName, 0, sizeof m_PeerName);
    m_Type = TCP_SOCK;
    m_Protocol = 0;
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    create();
#else
    if(create() == SOCKET_ERROR)
    {
        IsOk = false;
    }
#endif
}

StreamSocket::StreamSocket(SOCKET sock)
{
    m_GotSockName = false;
    m_GotPeerName = false;
    m_Type = TCP_SOCK;
    m_Protocol = 0;
    m_Sock = sock;
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
sockaddr_in& StreamSocket::getSockName() throw(SockException)
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

sockaddr_in& StreamSocket::getPeerName() throw(SockException)
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

void StreamSocket::setTcpLevelOpt(int Opt, const char* Value, int OptLen) throw(SockException)
{
#ifndef __VXWORKS__
    if(setsockopt(m_Sock, IPPROTO_TCP, Opt, Value, OptLen) == SOCKET_ERROR)
    {
#else
    if(setsockopt(m_Sock, IPPROTO_TCP, Opt, const_cast <char*> (Value), OptLen) == SOCKET_ERROR)
    {
#endif
        SOCK_EXCEPT_THROW(WSAGetLastError());
    }
}


void StreamSocket::getTcpLevelOpt(int Opt, char* Value, int* OptLen) throw(SockException)
{
#ifndef __VXWORKS__
    if(getsockopt(m_Sock, IPPROTO_TCP, Opt, Value, (socklen_type*)OptLen) == SOCKET_ERROR)
    {
#else
    if(getsockopt(m_Sock, IPPROTO_TCP, Opt, const_cast <char*> (Value), (socklen_type*)OptLen) == SOCKET_ERROR)
    {
#endif
        SOCK_EXCEPT_THROW(WSAGetLastError());
    }
}
#else // _WITHOUT_SOCK_EXCEPTIONS
int StreamSocket::getSockName(sockaddr_in* SockAddr)
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

int StreamSocket::getPeerName(sockaddr_in* PeerAddr)
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

int StreamSocket::setTcpLevelOpt(int Opt, const char* Value, int OptLen)
{
#ifndef __VXWORKS__
    return setsockopt(m_Sock, IPPROTO_TCP, Opt, Value, OptLen);
#else
    return setsockopt(m_Sock, IPPROTO_TCP, Opt, const_cast <char*> (Value), OptLen);
#endif
}


int StreamSocket::getTcpLevelOpt(int Opt, char* Value, int* OptLen)
{
#ifndef __VXWORKS__
    return getsockopt(m_Sock, IPPROTO_TCP, Opt, Value, (socklen_type*)OptLen);
#else
    return getsockopt(m_Sock, IPPROTO_TCP, Opt, const_cast <char*> (Value), (socklen_type*)OptLen);
#endif
}

#endif // _WITHOUT_SOCK_EXCEPTIONS

void StreamSocket::disconnect(long Timeout)
{
    shutdown(m_Sock, 1);
    fd_set RdSet;
    fd_set ErrSet;
    FD_ZERO(&RdSet);
    FD_ZERO(&ErrSet);
    FD_SET(m_Sock, &RdSet);
    FD_SET(m_Sock, &ErrSet);
    timeval Time;
    Time.tv_sec = Timeout / 1000;
    Time.tv_usec = (Timeout - Time.tv_sec * 1000) * 1000;
    int SelRes = select(static_cast < int > (m_Sock) + 1, &RdSet, NULL, &ErrSet, &Time);
    if(SelRes > 0 && (FD_ISSET(m_Sock, &RdSet) || FD_ISSET(m_Sock, &ErrSet)))
    {
        char TmpBuf[16 * 1024];
        recv(m_Sock, TmpBuf, 16 * 1024, 0);
    }
}
