/* StreamSocketUds.h
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

#ifndef STREAMSOCKETUDS_H
#define STREAMSOCKETUDS_H

#include <StreamSocket.h>
#include "UnixDomainSocket.h"

class SOCKLIB_API StreamSocketUds : virtual public UnixDomainSocket, virtual public StreamSocket
{
public:
    /// Constructor.
    /// \param sock : socket file descriptor/handle.
    StreamSocketUds(SOCKET Sock);

    /// Destructor.
    virtual ~StreamSocketUds();

#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Deliver bound endpoint properties.
    /// \return sockaddr_in structure filled with bound endpoint properties.
    /// \throw SockException.
    sockaddr_un& getSockName();
    /// Deliver connected endpoint properties.
    /// \return sockaddr_in structure filled with connected endpoint properties.
    /// \throw SockException.
    sockaddr_un& getPeerName();
#else
    int getSockName(sockaddr_un* SockAddr);
    int getPeerName(sockaddr_un* PeerAddr);
#endif

protected:
    sockaddr_un m_SockName; ///< Bound endpoint properties.
    sockaddr_un m_PeerName; ///< Destination endpoint properties.

    /// Constructor.
    /// \throw SockException.
    StreamSocketUds();

private:
    /// Constructor.
    /// Copy constructor. Makes the class uncopyable.
    StreamSocketUds(StreamSocketUds& s);
    /// Operator assign.
    /// Makes the class uncopyable.
    StreamSocketUds& operator=(StreamSocketUds& s);
};

#endif // STREAMSOCKETUDS_H
