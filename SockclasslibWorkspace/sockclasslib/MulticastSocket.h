/* MulticastSocket.h
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

#ifndef _MULTICASTSOCKET_H
#define _MULTICASTSOCKET_H

#include <DatagramSocket.h>

/// IPv4 datagram socket capable to send multicast datagrams.

class SOCKLIB_API MulticastSocket : public DatagramSocket
{
public:
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Constructor.
    /// \throw SockException.
    MulticastSocket() throw (SockException)
    {
    }
#else

    MulticastSocket()
    {
    }
#endif

#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Set Time-To-Live for sent multicasts.
    /// \param ttl : TTL number.
    /// \throw SockException.
    void setTTL(unsigned char ttl) throw (SockException);
    /// Set default network interface.
    /// \param Address : address of default network interface in network byte order.
    /// \throw SockException.
    void setDefaultIf(in_addr_t Address) throw (SockException);
    /// Set default network interface.
    /// \param Address : address of default network interface in decilmal dot notation.
    /// \throw SockException.
    void setDefaultIf(const char* Address) throw (SockException);
    /// Switch on or off loppback for sent multicasts.
    /// \param On : lopback on/off.
    /// \throw SockException.
    void setLoopBack(bool On = true) throw (SockException);
#else
    int setTTL(unsigned char ttl);
    int setDefaultIf(in_addr_t Address = INADDR_ANY);
    int setDefaultIf(const char* Address);
    int setLoopBack(bool On = true);
#endif
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Connect the socket.
    /// Implements pure virtual function of base class.
    /// Has no meaning for this particular class.
    /// \throw SockException.
    virtual void connect() throw (SockException)
    {
        SOCK_EXCEPT_THROW("Wrong operation");
    }
#else

    virtual int connect()
    {
        return SOCKET_ERROR;
    }
#endif

protected:

private:
    /// Copy constructor.
    /// Makes the class uncopyable.
    MulticastSocket(MulticastSocket& s);
    /// Operator assign.
    /// Makes the class uncopyable.
    MulticastSocket& operator=(MulticastSocket&);
};

#endif /* _MULTICASTSOCKET_H */
