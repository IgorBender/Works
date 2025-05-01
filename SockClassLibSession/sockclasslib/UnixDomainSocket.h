/* UnixDomainSocket.h
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

#ifndef UNIXDOMAINSOCKET_H
#define UNIXDOMAINSOCKET_H

#include "SocketClass.h"
#include <sys/un.h>

/// UNIX domain socket base abstract class.
/// Is base for all kinds of UDS sockets.
class SOCKLIB_API UnixDomainSocket : virtual public SocketClass
{
public:
    /// Constructor.
    /// Default constructor.
    UnixDomainSocket(SockType type = NO_TYPE, int protocol = 0)
    {
        m_Domain = UNIX_DOMAIN;
        m_Type = type;
        m_Protocol = protocol;
    }

    virtual ~UnixDomainSocket();

protected:
    /// Constructor.
    /// Constructs socket on given socket file descriptor/handle.
    /// \param sock : socket file descriptor/handle.
    /// \param type : socket type.
    /// \param protocol : socket protocol.
    UnixDomainSocket(int sock, SockType type = NO_TYPE, int protocol = 0)
    {
        m_Sock = sock;
        m_Domain = UNIX_DOMAIN;
        m_Type = type;
        m_Protocol = protocol;
    }
};

#endif // UNIXDOMAINSOCKET_H
