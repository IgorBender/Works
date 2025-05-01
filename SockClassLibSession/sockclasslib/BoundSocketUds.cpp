/*
 *  BoundSocketUds.cpp
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
