/* StreamSocketV6.h
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

#ifndef STREAMSOCKETV6_H_
#define STREAMSOCKETV6_H_

#ifndef _NO_IPV6

#include <StreamSocket.h>
#include <InternetSocketV6.h>

/// Stream socket class for IPv6.
/// The base class for all stream-oriented sockets.
class SOCKLIB_API StreamSocketV6 : virtual public InternetSocketV6, virtual public StreamSocket
{
public:
    /// Constructor.
    /// \param sock : socket file descriptor/handle.
	StreamSocketV6(SOCKET sock);

	/// Destructor.
	virtual ~StreamSocketV6()
    {
        close();
    }

#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Deliver bound endpoint properties.
    /// \return sockaddr_in structure filled with bound endpoint properties.
    /// \throw SockException.
    sockaddr_in6& getSockName();
    /// Deliver connected endpoint properties.
    /// \return sockaddr_in structure filled with connected endpoint properties.
    /// \throw SockException.
    sockaddr_in6& getPeerName();
#else
    int getSockName(sockaddr_in6* SockAddr);
    int getPeerName(sockaddr_in6* PeerAddr);
#endif

protected:
	sockaddr_in6 m_SockName; ///< Bound endpoint properties.
	sockaddr_in6 m_PeerName; ///< Destination endpoint properties.

    /// Constructor.
    StreamSocketV6();

private:
    /// Constructor.
    /// Copy constructor. Makes the class uncopyable.
    StreamSocketV6(StreamSocketV6& s);
    /// Operator assign.
    /// Makes the class uncopyable.
    StreamSocketV6& operator=(StreamSocketV6& s);
};

#endif /* NO_IPV6 */
#endif /* STREAMSOCKETV6_H_ */
