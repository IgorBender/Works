/* ConnectedSocketUds.h
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

#ifndef CONNECTEDSOCKETUDS_H
#define CONNECTEDSOCKETUDS_H

#include "UnixDomainSocket.h"
#include <DisconnectException.h>

/// Socket that can connect to another.
class SOCKLIB_API ConnectedSocketUds : virtual public UnixDomainSocket
{
public:
    /// Constructor.
    /// Default constructor.
    ConnectedSocketUds()
    {
        memset(&m_Destination, 0, sizeof m_Destination);
        m_Destination.sun_family = NO_DOMAIN;
        m_Connected = false;
    }
    /// Set connection destination endpoint.
    /// \param Dest : sockaddr_in structure filled with destination endpoint properties.
    void setDestination(sockaddr_un& Dest)
    {
        m_Destination.sun_family = Dest.sun_family;
        strncpy(m_Destination.sun_path, Dest.sun_path, sizeof(m_Destination.sun_path));
    }
    /// Set connection destination endpoint.
    /// \param Path : Socket path.
    void setDestination(const std::string& Path)
    {
        m_Destination.sun_family = UNIX_DOMAIN;
        Path.copy(m_Destination.sun_path, sizeof(m_Destination.sun_path));
    }

    /// Deliver destination endpoint properties.
    /// \return sockaddr_in structure filled with endpoint properties.
    sockaddr_un& getDestination()
    {
        return m_Destination;
    }
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Connect to destination endpoint.
    /// \throw SockException.
    virtual void connect();

    /// Connect to destination endpoint.
    /// \param Path : Socket path.
    /// \throw SockException.
    void connect(const std::string& Path)
    {
        setDestination(Path);
        connect();
    }
    /// Connect to destination endpoint.
    /// \param Dest : sockaddr_in structure filled with destination endpoint properties.
    /// \throw SockException.
    void connect(sockaddr_un& Dest)
    {
        setDestination(Dest);
        connect();
    }
#else // _WITHOUT_SOCK_EXCEPTIONS
    virtual int connect();
    int connect(const std::string& Path)
    {
        setDestination(Path);
        return connect();
    }
    int connect(sockaddr_un& Dest)
    {
        setDestination(Dest);
        return connect();
    }
#endif // _WITHOUT_SOCK_EXCEPTIONS

protected:
    sockaddr_un m_Destination; ///< Destination endpoint properties.

private:
    /// Copy constructor.
    /// Makes the class uncopyable .
    ConnectedSocketUds(ConnectedSocketUds& s);
    /// Operator assign.
    /// Makes the class uncopyable .
    ConnectedSocketUds& operator= (ConnectedSocketUds& s);
};

#endif // CONNECTEDSOCKETUDS_H
