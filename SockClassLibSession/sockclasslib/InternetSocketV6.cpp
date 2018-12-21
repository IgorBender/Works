/* InternetSocketV6.cpp
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

#ifndef _NO_IPV6

#include <InternetSocketV6.h>

InternetSocketV6::~InternetSocketV6()
{}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void InternetSocketV6::setIpLevelOpt(int Opt, const char* Value,
                                     socklen_type OptLen)
#else
int InternetSocketV6::setIpLevelOpt(int Opt, const char* Value,
                                    socklen_type OptLen)
#endif
{
    if(setsockopt(m_Sock, IPPROTO_IPV6, Opt,
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
void InternetSocketV6::getIpLevelOpt(int Opt, char* Value,
                                     socklen_type* OptLen)
#else
int InternetSocketV6::getIpLevelOpt(int Opt, char* Value,
                                    socklen_type* OptLen)
#endif
{
    if(getsockopt(m_Sock, IPPROTO_IPV6, Opt,
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

#endif /* _NO_IPV6 */
