/* ServerSimple.cpp
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

#include <ServerSimple.h>

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void ServerSimple::listen(int Backlog) throw(SockException)
{
    if(::listen(m_Sock, Backlog) == SOCKET_ERROR)
    {
        SOCK_EXCEPT_THROW(WSAGetLastError());
    }
}

StreamSocketV4* ServerSimple::accept() throw(SockException)
{
    socklen_type Len = sizeof m_PeerName;
    SOCKET Res = ::accept(m_Sock,
                       reinterpret_cast <sockaddr*> (&m_PeerName), &Len);
    if(Res == INVALID_SOCKET)
    {
        SOCK_EXCEPT_THROW(WSAGetLastError());
    }
    StreamSocketV4* RetSock = new StreamSocketV4(Res);
    RetSock->setConnected(true);
    return RetSock;
}
#else
int ServerSimple::listen(int Backlog)
{
    return ::listen(m_Sock, Backlog);
}

StreamSocketV4* ServerSimple::accept()
{
    socklen_type Len = sizeof m_PeerName;
    SOCKET Res = ::accept(m_Sock,
                       reinterpret_cast <sockaddr*> (&m_PeerName), &Len);
    if(Res == INVALID_SOCKET)
    {
        return NULL;
    }
    StreamSocketV4* RetSock = new StreamSocketV4(Res);
    RetSock->setConnected(true);
    return RetSock;
}
#endif

