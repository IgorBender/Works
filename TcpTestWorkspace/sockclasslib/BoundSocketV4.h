/* BoundSocketV4.h
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

#ifndef _BOUNDSOCKETV4_H
#define _BOUNDSOCKETV4_H

#include <InternetSocketV4.h>

/// Socket bounded to particular port and IPv4 address (end-point).

class SOCKLIB_API BoundSocketV4 : public InternetSocketV4
{
public:
    /// Constructor.
    /// Default constructor.
    BoundSocketV4();

    /// Destructor
    /// Empty virtual destructor for warning elimination in Eclipse
    virtual ~BoundSocketV4()
    {}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Bind socket to endpoint.
    /// \throw SockException.
    virtual void bind() throw (SockException);
#else
    virtual int bind();
#endif

    /// Set endpoint for bind the socket.
    /// \param Addr : sockaddr_in structure filled with endpoint properties.
    virtual void setEndPoint(sockaddr_in Addr)
    {
        m_EndPoint.sin_family = Addr.sin_family;
        m_EndPoint.sin_addr.s_addr = Addr.sin_addr.s_addr;
        m_EndPoint.sin_port = Addr.sin_port;
    }
    /// Set endpoint for bind the socket.
    /// \param Address : IPv4 address in network byte order.
    /// \param Port : port number in network byte order.
    virtual void setEndPoint(in_addr_t Address, short Port);
#ifndef _WITHOUT_SOCK_EXCEPTIONS

    /// Set endpoint for bind the socket.
    /// \param Address : IPv4 address in decimal dot notation.
    /// \param Port : port number in network byte order.
    virtual void setEndPoint(const char* Address, short Port) throw (SockException);
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
    sockaddr_in m_EndPoint; ///< Socket's bound endpoint.
    bool m_Bound; ///< Bound status.
};

#endif

