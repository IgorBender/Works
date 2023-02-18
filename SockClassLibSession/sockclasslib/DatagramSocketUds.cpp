/* DatagramSocketUds.cpp
 *
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

#include "DatagramSocketUds.h"

DatagramSocketUds::DatagramSocketUds(int protocol) : ConnectedSocketUds()
{
    m_Type = UDP_SOCK;
    m_Protocol = protocol;
    memset(&m_Source, 0, sizeof m_Source); // m_Destination is initialized in base class
    m_Source.sun_family = NO_DOMAIN;
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    create();
#else
    if(create() == SOCKET_ERROR)
    {
        IsOk = false;
        return;
    }
#endif
}

DatagramSocketUds::~DatagramSocketUds()
{
    close();
}

int DatagramSocketUds::sendTo(const void* Buffer, size_t Length, int Flags)
{
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    if(m_Destination.sun_family == NO_DOMAIN)
    {
        SOCK_EXCEPT_THROW("Wrong socket domain", m_Sock);
    }
#endif
    if(Length == 0)
        return 0;

    int Result;

    if((Result = static_cast<int>(sendto(m_Sock,
#ifndef __VXWORKS__
                        reinterpret_cast < const char* > (Buffer),
#else
                        const_cast < char* > (reinterpret_cast < const char* > (Buffer)),
#endif
#ifndef _MSC_VER
                        Length,
#else
                        static_cast < int > (Length),
#endif
                        Flags, reinterpret_cast < struct sockaddr* > (&m_Destination),
                        sizeof m_Destination))) == SOCKET_ERROR)

    {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        SOCK_EXCEPT_THROW(WSAGetLastError(), m_Sock);
#endif

    }
    return Result;
}

int DatagramSocketUds::receiveFrom(void* Buffer, size_t Length, int Flags)
{
    if(Length == 0)
        return 0;

    int Result;
    socklen_type Size = sizeof(struct sockaddr_un);
    memset(&m_Source, '\0', Size);
    if((Result = static_cast<int>(recvfrom(m_Sock, reinterpret_cast < char* > (Buffer),
                      #ifndef _MSC_VER
                                              Length,
                      #else
                                              static_cast < int > (Length),
                      #endif
                          Flags, reinterpret_cast <struct sockaddr*> (&m_Source),
                          &Size))) == SOCKET_ERROR)
    {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        SOCK_EXCEPT_THROW(WSAGetLastError(), m_Sock);
#endif

    }
    return Result;
}

