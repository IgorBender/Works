/* ServerSimpleUds.cpp
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

#include "ServerSimpleUds.h"

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void ServerSimpleUds::listen(int Backlog)
{
    if(::listen(m_Sock, Backlog) == SOCKET_ERROR)
    {
        SOCK_EXCEPT_THROW(WSAGetLastError(), m_Sock);
    }
}

StreamSocketUds* ServerSimpleUds::accept()
{
    socklen_type Len = sizeof m_PeerName;
    SOCKET Res = ::accept(m_Sock,
                       reinterpret_cast <sockaddr*> (&m_PeerName), &Len);
    if(Res == INVALID_SOCKET)
    {
        SOCK_EXCEPT_THROW(WSAGetLastError(), m_Sock);
    }
    StreamSocketUds* RetSock = new StreamSocketUds(Res);
    RetSock->setConnected(true);
    return RetSock;
}
#else
int ServerSimpleUds::listen(int Backlog)
{
    return ::listen(m_Sock, Backlog);
}

StreamSocketUds* ServerSimpleUds::accept()
{
    socklen_type Len = sizeof m_PeerName;
    SOCKET Res = ::accept(m_Sock,
                       reinterpret_cast <sockaddr*> (&m_PeerName), &Len);
    if(Res == INVALID_SOCKET)
    {
        return nullptr;
    }
    StreamSocketUds* RetSock = new StreamSocketUds(Res);
    RetSock->setConnected(true);
    return RetSock;
}
#endif

