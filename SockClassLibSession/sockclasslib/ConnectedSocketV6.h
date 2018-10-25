/* ConnectedSocketV6.h
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

#ifndef CONNECTEDSOCKETV6_H_
#define CONNECTEDSOCKETV6_H_

#ifndef _NO_IPV6

#include <InternetSocketV6.h>
#include <DisconnectException.h>

/// Socket that can connect to another.
class SOCKLIB_API ConnectedSocketV6 : virtual public InternetSocketV6
{
public:
    /// Constructor.
    /// Default constructor.
    ConnectedSocketV6()
    {
        memset(&m_Destination, 0, sizeof m_Destination);
        m_Destination.sin6_family = NO_DOMAIN;
        m_Connected = false;
    }
    /// Set connection destination endpoint.
    /// \param Dest : sockaddr_in6 structure filled with destination endpoint properties.
    void setDestination(sockaddr_in6 Dest)
    {
        m_Destination.sin6_family = Dest.sin6_family;
        memcpy(&m_Destination.sin6_addr, &Dest.sin6_addr, sizeof(in6_addr));
        m_Destination.sin6_port = Dest.sin6_port;
    }
    /// Set connection destination endpoint.
    /// \param Address : IPv6 address structure.
    /// \param Port : port number in network byte order.
    void setDestination(in6_addr Address, short Port);
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Set connection destination endpoint.
    /// \param Address : IPv6 address in textual notation.
    /// \param Port : port number in network byte order.
    void setDestination(const char* Address, short Port);
#else
    int setDestination(const char* Address, short Port);
#endif
    /// Deliver destination endpoint properties.
    /// \return sockaddr_in structure filled with endpoint properties.
    sockaddr_in6& getDestination()
    {
        return m_Destination;
    }
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Connect to destination endpoint.
    /// \throw SockException.
    virtual void connect();

    /// Connect to destination endpoint.
    /// \param Address : IPv6 address structure.
    /// \param Port : port number in network byte order.
    /// \throw SockException.
    void connect(in6_addr Address, short Port)
    {
        setDestination(Address, Port);
        connect();
    }
    /// Connect to destination endpoint.
    /// \param Address : IPv6 address in textual notation.
    /// \param Port : port number in network byte order.
    /// \throw SockException.
    void connect(const char* Address, short Port)
    {
        setDestination(Address, Port);
        connect();
    }
    /// Connect to destination endpoint.
    /// \param Dest : sockaddr_in6 structure filled with destination endpoint properties.
    /// \throw SockException.
    void connect(sockaddr_in6 Dest)
    {
        setDestination(Dest);
        connect();
    }
#else // _WITHOUT_SOCK_EXCEPTIONS
    virtual int connect();
    int connect(in6_addr Address, short Port)
    {
        setDestination(Address, Port);
        return connect();
    }
    int connect(const char* Address, short Port)
    {
        if(setDestination(Address, Port) == SOCKET_ERROR)
        {
            return SOCKET_ERROR;
        }
        return connect();
    }
    int connect(sockaddr_in6 Dest)
    {
        setDestination(Dest);
        return connect();
    }
#endif // _WITHOUT_SOCK_EXCEPTIONS

protected:
    sockaddr_in6 m_Destination; ///< Destination endpoint properties.

private:
    /// Copy constructor.
    /// Makes the class uncopyable .
    ConnectedSocketV6(ConnectedSocketV6& s);
    /// Operator assign.
    /// Makes the class uncopyable .
    ConnectedSocketV6& operator= (ConnectedSocketV6& s);
};

#endif /* _NO_IPV6 */
#endif /* CONNECTEDSOCKETV6_H_ */
