/* InternetSocketV6.h
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

#ifndef INTERNETSOCKETV6_H
#define INTERNETSOCKETV6_H

#ifdef _WIN32
#pragma warning( disable : 4290 )
#pragma warning( disable : 4250 )

#endif /* _WIN32 */

#ifndef _NO_IPV6

#include <SocketClass.h>
#include <DisconnectException.h>

/// IPv6 base abstract class.
/// Is base for all kinds of IPv6 sockets.
class SOCKLIB_API InternetSocketV6 : virtual public SocketClass
{
public:
    /// Constructor.
    /// Default constructor.
    InternetSocketV6(SockType type = NO_TYPE, int protocol = 0)
    {
#ifndef _NO_IPV6
    	m_Domain = INET6_DOMAIN;
        m_Type = type;
        m_Protocol = protocol;
#else
    m_Domain = NO_DOMAIN;

#ifndef _WITHOUT_SOCK_EXCEPTIONS
    SOCK_EXCEPT_THROW("No IPv6 support defined");
#else
    return;
#endif
#endif
    }

    virtual ~InternetSocketV6();

#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Set IPv6 level option.
    /// \param Opt : option type.
    /// \param Value : option value.
    /// \param OptLen : option value length.
    /// \throw SockException
    void setIpLevelOpt(int Opt, const char* Value, socklen_type OptLen);
    /// Deliver IPv6 level option.
    /// \param Opt : option type.
    /// \param Value : option value.
    /// \param OptLen : option value length.
    /// \throw SockException
    void getIpLevelOpt(int Opt, char* Value, socklen_type* OptLen);
#else
    int setIpLevelOpt(int Opt, const char* Value, socklen_type OptLen);
    int getIpLevelOpt(int Opt, char* Value, socklen_type* OptLen);
#endif

protected:
    /// Constructor.
    /// Constructs socket on given socket file descriptor/handle.
    /// \param sock : socket file descriptor/handle.
    /// \param type : socket type.
    /// \param protocol : socket protocol.
    InternetSocketV6(int sock, SockType type = NO_TYPE, int protocol = 0)
    {
        m_Sock = sock;
        m_Domain = INET6_DOMAIN;
        m_Type = type;
        m_Protocol = protocol;
    }
};

#endif /* _NO_IPV6 */
#endif /* INTERNETSOCKETV6_H */
