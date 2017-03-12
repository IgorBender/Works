/* SocketClass.h
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

#ifndef _SOCKETCLASS_H
#define _SOCKETCLASS_H

#ifndef _WIN32

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#else /* _WIN32 */

#pragma warning( disable : 4290 )
#pragma warning( disable : 4250 )

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#if !(_MSC_VER >= 1300)
#ifdef _AFXDLL
#include <afxwin.h>
#else
#include <windows.h>
#endif
//#include <windows.h>
#endif

#endif /* _WIN32 */

#include <DisconnectException.h>
#include <sockclasslib.h>

///
/// Base abstract class for all types of sockets.
/// This class defines abstract socket, that cannot be instantiated.
/// 

class SOCKLIB_API SocketClass
{
public:

    /// Domain enumeration, actually there are much more domains (address families),
    /// but we are interested in these domains only.
    enum SockDomain
    {
        NO_DOMAIN = AF_UNSPEC, ///< No-specified domain, should not be used
        UNIX_DOMAIN = AF_UNIX, ///< UNIX domain
        INTERNET_DOMAIN = AF_INET, ///< IPv4
        IPX_DOMAIN = AF_IPX ///< IPX
#ifndef _WIN32
        ,
        ROUTING_DOMAIN = AF_ROUTE ///< Routing
#endif /* _WIN32 */
#ifndef __VXWORKS__
        ,
        INET6_DOMAIN = AF_INET6 ///< IPv6
#endif /* __VXWORKS__ */
    };

    /// Socket type enumeration.
    enum SockType
    {
        NO_TYPE = -1, ///< No type sentinel
        UDP_SOCK = SOCK_DGRAM, ///< Datagram sockets (UDP)
        TCP_SOCK = SOCK_STREAM, ///< Stream sockets (TCP, SCTP)
        RAW_SOCK = SOCK_RAW ///< Raw sockets
    };

    /// Constructor.
    /// \details Default constructor.
    SocketClass(SockDomain domain = NO_DOMAIN, SockType type = NO_TYPE,
                int protocol = 0) :
    m_Sock(0), m_Domain(domain), m_Type(type), m_Protocol(protocol), m_Flags(0), m_Connected(false)
    {}


    /// Sets desired socket domain.
    /// \param Domain : desired socket domain (Address Family).
    void setDomain(SockDomain Domain)
    {
        m_Domain = Domain;
    }
    /// Delivers defined socket domain.
    /// \return Defined socket domain.
    SockDomain getDomain()
    {
        return m_Domain;
    }
    /// Sets socket type (Datagram, Stream or Raw).
    /// \param Type : desired socket type
    void setType(SockType Type)
    {
        m_Type = Type;
    }
    /// Delivers defined socket type.
    /// \return defined socket type.
    SockType getType()
    {
        return m_Type;
    }
    /// Sets desired socket protocol.
    /// It is convinient to set socket protocol other than 0 (default) in case
    /// of RAW socket or STREAM socket if SCTP end-point is required.
    /// \param Protocol : specific protocol or 0 for default.
    void setProtocol(int Protocol)
    {
        m_Protocol = Protocol;
    }
    /// Delivers defined protocol.
    /// \return defined protocol.
    int getProtocol()
    {
        return m_Protocol;
    }
    /// Delivers socket file descriptor/handle.
    /// \return socket file descriptor/handle.
    SOCKET getSock()
    {
        return m_Sock;
    }
    /// Sets non-blocking mode of the socket on/off
    /// \param On : non-blocking mode.
    /// \throw SockException
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Set socket non-blocking
    /// \param On : on/off socket non-blocking mode
    /// \throw SockException
    void setNonBlockMode(bool On) throw (SockException);
    /// Set socket level option.
    /// \param Opt : option type.
    /// \param Value : option value.
    /// \param OptLen : option value length.
    /// \throw SockException
    void setSockLevelOpt(int Opt, const char* Value, int OptLen) throw (SockException);
    /// Deliver socket level option.
    /// \param Opt : option type.
    /// \param Value : option value.
    /// \param OptLen : option value length.
    /// \throw SockException
    void getSockLevelOpt(int Opt, char* Value, int* OptLen) throw (SockException);
    /// Send data via socket
    /// \param Buffer : data buffer.
    /// \param Length : data size.
    /// \param Flags : sending flags.
    /// \throw SockException
#ifndef __linux__
    virtual int send(const void* Buffer, size_t Length, int Flags = 0) throw (SockException, DisconnectException);
#else
    virtual int send(const void* Buffer, size_t Length, int Flags = MSG_NOSIGNAL) throw (SockException, DisconnectException);
#endif
    /// Receive data from socket
    /// \param Buffer : data buffer for received data.
    /// \param Length : amount of data potentially to be received.
    /// \param Flags : receiving flags.
    /// \throw SockException
    virtual int receive(void* Buffer, size_t Length, int Flags = 0) throw (SockException, DisconnectException);
#else
    int setNonBlockMode(bool On);
    int setSockLevelOpt(int Opt, const char* Value, int OptLen);
    int getSockLevelOpt(int Opt, char* Value, int* OptLen);
#ifndef __linux__
    virtual int send(const void* Buffer, size_t Length, int Flags = 0);
#else
    virtual int send(const void* Buffer, size_t Length, int Flags = MSG_NOSIGNAL);
#endif
    virtual int receive(void* Buffer, size_t Length, int Flags = 0);
#endif

    /// Deliver connected state.
    /// \return connected state.
    bool isConnected()
    {
        return m_Connected;
    }
    /// Set connected state.
    /// \param On : connected state on/off.
    void setConnected(bool On)
    {
        m_Connected = On;
    }

protected:
    /// Constructor.
    /// Constructor based on socket file descriptor/handle.
    SocketClass(int sock, SockDomain domain = NO_DOMAIN,
                SockType type = NO_TYPE, int protocol = 0) :
    m_Sock(sock), m_Domain(domain), m_Type(type), m_Protocol(protocol), m_Flags(0), m_Connected(false)
    {}

    /// Destructor
    virtual ~SocketClass()
    {}

    /// Creates socket.
    /// \throw SockException.
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    void create() throw (SockException);
#else
    int create();
#endif

    /// Closes the socket.
    /// \anchor close
    virtual void close() = 0;

    SOCKET m_Sock; ///< Socket file descriptor/handle.
    SockDomain m_Domain; ///< Defined socket domain (Address Family).
    SockType m_Type; ///< Defined socket type.
    int m_Protocol; ///< Defined socket Protocol
    /// Socket flags to be saved (for non-blocking mode switch).
#ifndef _WIN32
    int m_Flags;
#else
    unsigned long m_Flags;
#endif
    bool m_Connected; ///< Connected state.
};

#endif /* _SOCKETCLASS_H */

