/* ConnectedSocketV4.h
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

#ifndef CONNECTEDSOCKETV4_H
#define CONNECTEDSOCKETV4_H

#include <InternetSocketV4.h>
#include <DisconnectException.h>

/// Socket that can connect to another.
class SOCKLIB_API ConnectedSocketV4 : virtual public InternetSocketV4
{
public:
    /// Constructor.
    /// Default constructor.
    ConnectedSocketV4() : InternetSocketV4()
    {
        memset(&m_Destination, 0, sizeof m_Destination);
        m_Destination.sin_family = NO_DOMAIN;
        m_Connected = false;
    }
    /// Set connection destination endpoint.
    /// \param Dest : sockaddr_in structure filled with destination endpoint properties.
    void setDestination(sockaddr_in Dest)
    {
        m_Destination.sin_family = Dest.sin_family;
        m_Destination.sin_addr.s_addr = Dest.sin_addr.s_addr;
        m_Destination.sin_port = Dest.sin_port;
    }
    /// Set connection destination endpoint.
    /// \param Address : IPv4 address in network byte order.
    /// \param Port : port number in network byte order.
    void setDestination(in_addr_t Address, short Port);
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Set connection destination endpoint.
    /// \param Address : IPv4 address in decimal dot notation.
    /// \param Port : port number in network byte order.
    void setDestination(const char* Address, short Port);
#else
    int setDestination(const char* Address, short Port);
#endif
    /// Deliver destination endpoint properties.
    /// \return sockaddr_in structure filled with endpoint properties.
    sockaddr_in& getDestination()
    {
        return m_Destination;
    }
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Connect to destination endpoint.
    /// \throw SockException.
    virtual void connect();
    
    /// Connect to destination endpoint.
    /// \param Address : IPv4 address in network byte order.
    /// \param Port : port number in network byte order.
    /// \throw SockException.
    void connect(in_addr_t Address, short Port)
    {
        setDestination(Address, Port);
        connect();
    }
    /// Connect to destination endpoint.
    /// \param Address : IPv4 address in decimal dot notation.
    /// \param Port : port number in network byte order.
    /// \throw SockException.
    void connect(const char* Address, short Port)
    {
        setDestination(Address, Port);
        connect();
    }
    /// Connect to destination endpoint.
    /// \param Dest : sockaddr_in structure filled with destination endpoint properties.
    /// \throw SockException.
    void connect(sockaddr_in Dest)
    {
        setDestination(Dest);
        connect();
    }
#else // _WITHOUT_SOCK_EXCEPTIONS
    virtual int connect();
    int connect(in_addr_t Address, short Port)
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
    int connect(sockaddr_in Dest)
    {
        setDestination(Dest);
        return connect();
    }
#endif // _WITHOUT_SOCK_EXCEPTIONS

protected:
    sockaddr_in m_Destination; ///< Destination endpoint properties.

private:
    /// Copy constructor.
    /// Makes the class uncopyable .
    ConnectedSocketV4(ConnectedSocketV4& s);
    /// Operator assign.
    /// Makes the class uncopyable .
    ConnectedSocketV4& operator= (ConnectedSocketV4& s);
};

#endif // CONNECTEDSOCKETV4_H

