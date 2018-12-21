/* StreamSocket.h
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

#ifndef STREAMSOCKET_H
#define STREAMSOCKET_H


#include <SocketClass.h>
#include <DisconnectException.h>
#ifndef _WIN32
#if (__QNX__ || __VXWORKS__ || __linux__)
#include <netinet/tcp.h>
#else
#include <inet/tcp.h>
#endif
#endif


class ServerSimple;
class ServerSimpleV6;

/// Stream socket class.
/// The base class for all stream-oriented sockets.
class SOCKLIB_API StreamSocket : virtual public SocketClass
{
public:
    /// Constructor.
    /// \param sock : socket file descriptor/handle.
    StreamSocket(SOCKET sock);

    /// Destructor.
    virtual ~StreamSocket();

//#ifndef _WITHOUT_SOCK_EXCEPTIONS
//    /// Deliver bound endpoint properties.
//    /// \return sockaddr_in structure filled with bound endpoint properties.
//    /// \throw SockException.
//    sockaddr_in& getSockName();
//    /// Deliver connected endpoint properties.
//    /// \return sockaddr_in structure filled with connected endpoint properties.
//    /// \throw SockException.
//    sockaddr_in& getPeerName();
//#else
//    int getSockName(sockaddr_in* SockAddr);
//    int getPeerName(sockaddr_in* PeerAddr);
//#endif
    /// Set socket file descriptor/handle.
    /// \param i : socket file descriptor/handle.
    void setSock(int i)
    {
        m_Sock = i;
    }
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Set TCP level option.
    /// \param Opt : option type.
    /// \param Value : option value.
    /// \param OptLen : option value length.
    /// \throw SockException
    void setTcpLevelOpt(int Opt, const char* Value, socklen_type OptLen);
    /// Deliver TCP level option.
    /// \param Opt : option type.
    /// \param Value : option value.
    /// \param OptLen : option value length.
    /// \throw SockException
    void getTcpLevelOpt(int Opt, char* Value, socklen_type* OptLen);
#else
    int setTcpLevelOpt(int Opt, const char* Value, socklen_type OptLen);
    int getTcpLevelOpt(int Opt, char* Value, socklen_type* OptLen);
#endif
    /// Disconnect endpoint.
    /// \param Timeout : Time to wait for orderly close the socket in milliseconds.
    void disconnect(long Timeout = 0);

protected:
//    sockaddr_in m_SockName; ///< Bound endpoint properties.
    bool m_GotSockName; ///< Bound endpoint properties are filled.
//    sockaddr_in m_PeerName; ///< Destination endpoint properties.
    bool m_GotPeerName; ///< Destination endpoint properties are filled.
    friend class ServerSimple;
    friend class ServerSimpleV6;

    /// Constructor.
    StreamSocket();
#ifdef _WITHOUT_SOCK_EXCEPTIONS

    bool isOk()
    {
        return IsOk;
    }
#endif
    /// Close the socket.
    virtual void close()
    {
        shutdown(m_Sock, 2);
        closesocket(m_Sock);
    }

#ifdef _WITHOUT_SOCK_EXCEPTIONS
    bool IsOk;
#endif

private:
    /// Constructor.
    /// Copy constructor. Makes the class uncopyable.
    StreamSocket(StreamSocket& s);
    /// Operator assign.
    /// Makes the class uncopyable.
    StreamSocket& operator=(StreamSocket& s);
};

#endif /* _STREAMSOCKET_H */

