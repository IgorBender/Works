/*
 *  BoundSocketUds.cpp
 *
 *  Created on: 2023-02-17
 *  Author: Igor Bender
 *
 */
#include "BoundSocketUds.h"

BoundSocketUds::BoundSocketUds()
{
    memset(&m_EndPoint, 0, sizeof m_EndPoint);
    m_EndPoint.sun_family = NO_DOMAIN;
    m_Bound = false;
}

void BoundSocketUds::setEndPoint(const std::string& Path)
{
    m_EndPoint.sun_family = UNIX_DOMAIN;
    Path.copy(m_EndPoint.sun_path, sizeof(m_EndPoint.sun_path));
}


#ifndef _WITHOUT_SOCK_EXCEPTIONS
void BoundSocketUds::bind()
#else
int BoundSocketUds::bind()
#endif
{
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    if(m_EndPoint.sun_family != UNIX_DOMAIN)
    {
        SOCK_EXCEPT_THROW("Wrong socket domain", m_Sock);
    }
#endif
    unlink(m_EndPoint.sun_path);
    if(::bind(m_Sock, reinterpret_cast<struct sockaddr*>(&m_EndPoint), sizeof m_EndPoint) == SOCKET_ERROR)
    {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        SOCK_EXCEPT_THROW(WSAGetLastError(), m_Sock);
#else
        return SOCKET_ERROR;
#endif
    }
    m_Bound = true;
#ifdef _WITHOUT_SOCK_EXCEPTIONS
    return 0;
#endif
}
