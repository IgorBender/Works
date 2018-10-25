/* BoundSocketV6.h
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

#ifndef BOUNDSOCKETV6_H_
#define BOUNDSOCKETV6_H_

#ifndef _NO_IPV6

#include <InternetSocketV6.h>

/// Socket bounded to particular port and IPv6 address (end-point).

class SOCKLIB_API BoundSocketV6 : virtual public InternetSocketV6
{
public:
    /// Constructor.
    /// Default constructor.
    BoundSocketV6();

    /// Destructor
    /// Empty virtual destructor for warning elimination in Eclipse
    virtual ~BoundSocketV6()
    {}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Bind socket to endpoint.
    /// \throw SockException.
    virtual void bind();
#else
    virtual int bind();
#endif

    /// Set endpoint for bind the socket.
    /// \param Addr : sockaddr_in6 structure filled with endpoint properties.
    virtual void setEndPoint(sockaddr_in6 Addr)
    {
        m_EndPoint.sin6_family = Addr.sin6_family;
        memcpy(&m_EndPoint.sin6_addr, &Addr.sin6_addr, sizeof(in6_addr));
        m_EndPoint.sin6_port = Addr.sin6_port;
    }
    /// Set endpoint for bind the socket.
    /// \param Address : IPv6 address in network byte order.
    /// \param Port : port number in network byte order.
    virtual void setEndPoint(in6_addr Address, short Port);
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Set endpoint for bind the socket.
    /// \param Address : IPv6 address textual notation.
    /// \param Port : port number in network byte order.
    virtual void setEndPoint(const char* Address, short Port);
#else
    virtual int setEndPoint(const char* Address, short Port);
#endif
    /// Is the socket bound already?
    /// \return Bound status.
    bool isBound()
    {
        return m_Bound;
    }

protected:
    sockaddr_in6 m_EndPoint; ///< Socket's bound endpoint.
    bool m_Bound; ///< Bound status.
};

#endif /* _NO_IPV6 */
#endif /* BOUNDSOCKETV6_H_ */
