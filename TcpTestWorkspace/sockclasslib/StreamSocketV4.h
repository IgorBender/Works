/* StreamSocketV4.h
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

#ifndef STREAMSOCKETV4_H_
#define STREAMSOCKETV4_H_

#include <StreamSocket.h>
#include <InternetSocketV4.h>

/// Stream socket class for IPv4.
/// The base class for IPv4 stream-oriented sockets.
class SOCKLIB_API StreamSocketV4 : virtual public InternetSocketV4, virtual public StreamSocket
{
public:
    /// Constructor.
    /// \param sock : socket file descriptor/handle.
    StreamSocketV4(SOCKET sock);

    /// Destructor.
    virtual ~StreamSocketV4()
    {
        close();
    }

#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Deliver bound endpoint properties.
    /// \return sockaddr_in structure filled with bound endpoint properties.
    /// \throw SockException.
    sockaddr_in& getSockName() throw (SockException);
    /// Deliver connected endpoint properties.
    /// \return sockaddr_in structure filled with connected endpoint properties.
    /// \throw SockException.
    sockaddr_in& getPeerName() throw (SockException);
#else
    int getSockName(sockaddr_in* SockAddr);
    int getPeerName(sockaddr_in* PeerAddr);
#endif

protected:
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Constructor.
    /// \throw SockException.
    StreamSocketV4() throw (SockException);
#else
    StreamSocketV4();
#endif
    sockaddr_in m_SockName; ///< Bound endpoint properties.
    sockaddr_in m_PeerName; ///< Destination endpoint properties.

private:
    /// Constructor.
    /// Copy constructor. Makes the class uncopyable.
    StreamSocketV4(StreamSocketV4& s);
    /// Operator assign.
    /// Makes the class uncopyable.
    StreamSocketV4& operator=(StreamSocketV4& s);
};

#endif /* STREAMSOCKETV4_H_ */
