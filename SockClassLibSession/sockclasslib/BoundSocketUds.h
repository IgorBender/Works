/* BoundSocketUds.h
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

#ifndef BOUNDSOCKETUDS_H
#define BOUNDSOCKETUDS_H

#include "UnixDomainSocket.h"

/// Socket bounded to particular path (end-point).

class SOCKLIB_API BoundSocketUds : virtual public UnixDomainSocket
{
public:
    /// Constructor.
    /// Default constructor.
    BoundSocketUds();

    /// Destructor
    virtual ~BoundSocketUds()
    {
        if(m_Bound)
            unlink(m_EndPoint.sun_path);
    }

#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Bind socket to endpoint.
    /// \throw SockException.
    virtual void bind();
#else
    virtual int bind();
#endif

    /// Set endpoint for bind the socket.
    /// \param Addr : sockaddr_un structure filled with endpoint properties.
    virtual void setEndPoint(sockaddr_un& Addr)
    {
        m_EndPoint.sun_family = Addr.sun_family;
        strncpy(m_EndPoint.sun_path, Addr.sun_path, sizeof(m_EndPoint.sun_path));
    }
    /// Set endpoint for bind the socket.
    /// \param Path : socket path.
    virtual void setEndPoint(const std::string& Path);
    /// Is the socket bound already?
    /// \return Bound status.
    bool isBound()
    {
        return m_Bound;
    }

protected:
    sockaddr_un m_EndPoint; ///< Socket's bound endpoint.
    bool m_Bound; ///< Bound status.
};

#endif // BOUNDSOCKETUDS_H
