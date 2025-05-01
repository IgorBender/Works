/* SocketClass.cpp
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

#include <errno.h>

#ifndef __QNX__
#include <string.h>
#endif

#ifndef _MSC_VER
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
        SOCK_EXCEPT_THROW("Wrong domain", -1);
    }

    if(m_Type == NO_TYPE)
    {
        SOCK_EXCEPT_THROW("Wrong type", -1);
    }

    if((m_Sock = socket(m_Domain, m_Type, m_Protocol)) == INVALID_SOCKET)
    {
        SOCK_EXCEPT_THROW(WSAGetLastError(), -1);
    }

#if ((!_MSC_VER) && (!__VXWORKS__))
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

#if ((!_MSC_VER) && (!__VXWORKS__))
    m_Flags = fcntl(m_Sock, F_GETFL, 0);
    return m_Flags;
#else
    return 0;
#endif
}
#endif // _WITHOUT_SOCK_EXCEPTIONS


#ifndef _WITHOUT_SOCK_EXCEPTIONS
void SocketClass::setNonBlockMode(bool On)
{
    int Result;
#ifndef _MSC_VER
    if(On)
    {
#ifndef __VXWORKS__
        m_Flags = fcntl(m_Sock, F_GETFL, 0);
        if(m_Flags < 0)
        {
            SOCK_EXCEPT_THROW(WSAGetLastError(), m_Sock);
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
            SOCK_EXCEPT_THROW(WSAGetLastError(), m_Sock);
        }
        Result = fcntl(m_Sock, F_SETFL, m_Flags & ~O_NONBLOCK);
#else
        Result = ioctl(m_Sock, FIONBIO, reinterpret_cast < int > (&On)); 
#endif
    }
    if(Result < 0)
    {
        SOCK_EXCEPT_THROW(WSAGetLastError(), m_Sock);
    }
#else
    m_Flags = On;
    Result = ioctlsocket(m_Sock, FIONBIO,&m_Flags);
    if(Result == SOCKET_ERROR)
    {
        SOCK_EXCEPT_THROW(WSAGetLastError(), m_Sock);
    }
#endif
}
#else // _WITHOUT_SOCK_EXCEPTIONS
int SocketClass::setNonBlockMode(bool On)
{
#ifndef _MSC_VER
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
                                     socklen_type  OptLen)
#else
int SocketClass::setSockLevelOpt(int Opt, const char* Value,
                                    socklen_type  OptLen)
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
        SOCK_EXCEPT_THROW(WSAGetLastError(), m_Sock);
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
                                     socklen_type* OptLen)
#else
int SocketClass::getSockLevelOpt(int Opt, char* Value,
                                    socklen_type* OptLen)
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
        SOCK_EXCEPT_THROW(WSAGetLastError(), m_Sock);
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
        SOCK_EXCEPT_THROW("Not connected", m_Sock);
#else
        return SOCKET_ERROR;
#endif
    }
    if(Length == 0)
        return 0;

    int Result = static_cast<int>(::send(m_Sock, reinterpret_cast < const char* > (Buffer),
#ifdef _MSC_VER
                        static_cast < int > (Length),
#else
                        Length,
#endif
                        Flags));
    if(Result == SOCKET_ERROR && WSAGetLastError() != SENDCONNRESET)
    {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        SOCK_EXCEPT_THROW(WSAGetLastError(), m_Sock);
#else
        return Result;
#endif
    }
    else if (Result == SOCKET_ERROR)
    {
        m_Connected = false;
#ifndef _WITHOUT_SOCK_EXCEPTIONS
#ifdef _MSC_VER

        DISCONN_EXCEPT_THROW(0, m_Sock);
#else // _MSC_VER
        DISCONN_EXCEPT_THROW(errno, m_Sock);
#endif // _MSC_VER
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
        SOCK_EXCEPT_THROW("Not connected", m_Sock);
#else
        return SOCKET_ERROR;
#endif
    }
    if(Length == 0)
        return 0;

    int Result = static_cast<int>(recv(m_Sock, reinterpret_cast < char* > (Buffer),
#ifdef _MSC_VER
                                       static_cast < int > (Length),
#else
                                       Length,
#endif
                                       Flags));

    if(Result == SOCKET_ERROR && WSAGetLastError() != RECVCONNRESET)
    {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        SOCK_EXCEPT_THROW(WSAGetLastError(), m_Sock);
#else
        return Result;
#endif
    }
    else if (Result == SOCKET_ERROR || Result == 0)
    {
        m_Connected = false;
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        DISCONN_EXCEPT_THROW("Brocken connection", m_Sock);
#endif
    }
    return Result;
}

