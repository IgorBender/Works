/* InternetSocketV4.cpp
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

#include <InternetSocketV4.h>

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void InternetSocketV4::setIpLevelOpt(int Opt, const char* Value,
                                     int OptLen) throw(SockException)
#else
int InternetSocketV4::setIpLevelOpt(int Opt, const char* Value,
                                    int OptLen)
#endif
{
    if(setsockopt(m_Sock, IPPROTO_IP, Opt,
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
void InternetSocketV4::getIpLevelOpt(int Opt, char* Value,
                                     int* OptLen) throw(SockException)
#else
int InternetSocketV4::getIpLevelOpt(int Opt, char* Value,
                                    int* OptLen)
#endif
{
    if(getsockopt(m_Sock, IPPROTO_IP, Opt,
#ifndef __VXWORKS__
                  Value,
#else
                  const_cast <char*> (Value),
#endif
                  (socklen_type*)OptLen) == SOCKET_ERROR)
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
