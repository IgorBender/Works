/* SocketClass.cpp
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

#include <errno.h>

#ifndef __QNX__
#include <string.h>
#endif

#ifndef _WIN32
#include <fcntl.h>
#endif

#ifdef __VXWORKS__
#include <ioLib.h>
#endif

#include <SocketClass.h>

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void SocketClass::create()
{
    if(m_Domain == NO_DOMAIN)
    {
        SOCK_EXCEPT_THROW("Wrong domain");
    }

    if(m_Type == NO_TYPE)
    {
        SOCK_EXCEPT_THROW("Wrong type");
    }

    if((m_Sock = socket(m_Domain, m_Type, m_Protocol)) == INVALID_SOCKET)
    {
        SOCK_EXCEPT_THROW(WSAGetLastError());
    }

#if ((!_WIN32) && (!__VXWORKS__))
    m_Flags = fcntl(m_Sock, F_GETFL, 0);
#endif
}
#else // _WITHOUT_SOCK_EXCEPTIONS
int SocketClass::create()
{
    if((m_Sock = socket(m_Domain, m_Type, m_Protocol)) == INVALID_SOCKET)
    {
        return SOCKET_ERROR;
    }

#if ((!_WIN32) && (!__VXWORKS__))
    m_Flags = fcntl(m_Sock, F_GETFL, 0);
    return m_Flags;
#endif
    return 0;
}
#endif // _WITHOUT_SOCK_EXCEPTIONS


#ifndef _WITHOUT_SOCK_EXCEPTIONS
void SocketClass::setNonBlockMode(bool On)
{
    int Result;
#ifndef _WIN32
    if(On)
    {
#ifndef __VXWORKS__
        m_Flags = fcntl(m_Sock, F_GETFL, 0);
        if(m_Flags < 0)
        {
            SOCK_EXCEPT_THROW(WSAGetLastError());
        }
        Result = fcntl(m_Sock, F_SETFL, m_Flags | O_NONBLOCK);
#else
        Result = ioctl(m_Sock, FIONBIO, reinterpret_cast < int > (&On)); 
#endif
    }
    else
    {
#ifndef __VXWORKS__
        m_Flags = fcntl(m_Sock, F_GETFL, 0);
        if(m_Flags < 0)
        {
            SOCK_EXCEPT_THROW(WSAGetLastError());
        }
        Result = fcntl(m_Sock, F_SETFL, m_Flags & ~O_NONBLOCK);
#else
        Result = ioctl(m_Sock, FIONBIO, reinterpret_cast < int > (&On)); 
#endif
    }
    if(Result < 0)
    {
        SOCK_EXCEPT_THROW(WSAGetLastError());
    }
#else
    m_Flags = On;
    Result = ioctlsocket(m_Sock, FIONBIO,&m_Flags);
    if(Result == SOCKET_ERROR)
    {
        SOCK_EXCEPT_THROW(WSAGetLastError());
    }
#endif
}
#else // _WITHOUT_SOCK_EXCEPTIONS
int SocketClass::setNonBlockMode(bool On)
{
#ifndef _WIN32
    int Result;
    if(On)
    {
        m_Flags = fcntl(m_Sock, F_GETFL, 0);
        if(m_Flags < 0)
        {
            return m_Flags;
        }
        Result = fcntl(m_Sock, F_SETFL, m_Flags | O_NONBLOCK);
    }
    else
    {
        Result = fcntl(m_Sock, F_SETFL, m_Flags & ~O_NONBLOCK);
    }
    return Result;
#else
    m_Flags = On;
    return(ioctlsocket(m_Sock, FIONBIO, &m_Flags));
#endif
}
#endif // _WITHOUT_SOCK_EXCEPTIONS

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void SocketClass::setSockLevelOpt(int Opt, const char* Value,
                                     unsigned int  OptLen)
#else
int SocketClass::setSockLevelOpt(int Opt, const char* Value,
                                    unsigned int  OptLen)
#endif
{
    if(setsockopt(m_Sock, SOL_SOCKET, Opt,
#ifndef __VXWORKS__
                  Value,
#else
                  const_cast <char*> (Value),
#endif
                  OptLen) == SOCKET_ERROR)
    {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        SOCK_EXCEPT_THROW(WSAGetLastError());
#else
        return SOCKET_ERROR;
#endif

    }
#ifdef _WITHOUT_SOCK_EXCEPTIONS
    return 0;
#endif
}


#ifndef _WITHOUT_SOCK_EXCEPTIONS
void SocketClass::getSockLevelOpt(int Opt, char* Value,
                                     unsigned int* OptLen)
#else
int SocketClass::getSockLevelOpt(int Opt, char* Value,
                                    unsigned int* OptLen)
#endif
{
    if(getsockopt(m_Sock, SOL_SOCKET, Opt,
#ifndef __VXWORKS__
                  Value,
#else
                  const_cast <char*> (Value),
#endif
                  OptLen) == SOCKET_ERROR)
    {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        SOCK_EXCEPT_THROW(WSAGetLastError());
#else
        return SOCKET_ERROR;
#endif

    }
#ifdef _WITHOUT_SOCK_EXCEPTIONS
    return 0;
#endif
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
int SocketClass::send(const void* Buffer, size_t Length, int Flags)
#else
int SocketClass::send(const void* Buffer, size_t Length, int Flags)
#endif
{
    if(!m_Connected)
    {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        SOCK_EXCEPT_THROW("Not connected");
#else
        return SOCKET_ERROR;
#endif
    }
    if(Length == 0)
        return 0;

    int Result = ::send(m_Sock, reinterpret_cast < const char* > (Buffer), static_cast < int > (Length), Flags);
    if(Result == SOCKET_ERROR && WSAGetLastError() != SENDCONNRESET)
    {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        SOCK_EXCEPT_THROW(WSAGetLastError());
#else
        return Result;
#endif
    }
    else if (Result == SOCKET_ERROR)
    {
        m_Connected = false;
#ifndef _WITHOUT_SOCK_EXCEPTIONS
#ifdef _WIN32

        DISCONN_EXCEPT_THROW(0);
#else // _WIN32
        DISCONN_EXCEPT_THROW(errno);
#endif // _WIN32
#endif // _WITHOUT_SOCK_EXCEPTIONS

    }

    return Result;
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
int SocketClass::receive(void* Buffer, size_t Length, int Flags)
#else
int SocketClass::receive(void* Buffer, size_t Length, int Flags)
#endif
{
    if(!m_Connected)
    {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        SOCK_EXCEPT_THROW("Not connected");
#else
        return SOCKET_ERROR;
#endif
    }
    if(Length == 0)
        return 0;

    int Result = recv(m_Sock, reinterpret_cast < char* > (Buffer), static_cast < int > (Length), Flags);

    if(Result == SOCKET_ERROR && WSAGetLastError() != RECVCONNRESET)
    {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        SOCK_EXCEPT_THROW(WSAGetLastError());
#else
        return Result;
#endif
    }
    else if (Result == SOCKET_ERROR || Result == 0)
    {
        m_Connected = false;
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        DISCONN_EXCEPT_THROW("Brocken connection");
#endif
    }
    return Result;
}

