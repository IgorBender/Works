/* MulticastSocketV6.h
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

#ifndef MULTICASTSOCKETV6_H
#define MULTICASTSOCKETV6_H

#ifndef _NO_IPV6

#include <DatagramSocketV6.h>
/// IPv6 datagram socket capable to send multicast datagrams.


class SOCKLIB_API MulticastSocketV6 : public DatagramSocketV6
{
public:
    /// Constructor.
    /// \throw SockException.
    MulticastSocketV6() : m_Index(0U)
    {}
    virtual ~MulticastSocketV6();

#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Set Time-To-Live for sent multicasts.
    /// \param ttl : TTL number.
    /// \throw SockException.
    void setTTL(unsigned char ttl);
    /// Set default network interface.
    /// \param Address : IPv6 address of default network interface.
    /// \throw SockException.
    void setDefaultIf(in6_addr Address);
    /// Set default network interface.
    /// \param Index : address of default network interface in textual dot notation.
    /// \throw SockException.
    void setDefaultIf(const char* Address);
    /// Switch on or off loppback for sent multicasts.
    /// \param On : lopback on/off.
    /// \throw SockException.
    void setLoopBack(bool On = true);
#else
    int setTTL(unsigned char ttl);
    int setDefaultIf(in6_addr Address);
    int setDefaultIf(const char* Address);
    int setLoopBack(bool On = true);
#endif
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Connect the socket.
    /// Implements pure virtual function of base class.
    /// Has no meaning for this particular class.
    /// \throw SockException.
    virtual void connect()
    {
        SOCK_EXCEPT_THROW("Wrong operation");
    }
#else
    virtual int connect()
    {
        return SOCKET_ERROR;
    }
#endif
    unsigned int getIndex()
    {
        return m_Index;
    }

protected:
    /// IPv6 interface index
    unsigned int m_Index;
private:
    /// Copy constructor.
    /// Makes the class uncopyable.
    MulticastSocketV6(MulticastSocketV6& s);
    /// Operator assign.
    /// Makes the class uncopyable.
    MulticastSocketV6& operator=(MulticastSocketV6&);

#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Set default network interface.
    /// \param Index : index of default network interface.
    /// \throw SockException.
    void setDefaultIf(unsigned int Index);
#else
    int setDefaultIf(unsigned int Index);
#endif
};

#endif /* _NO_IPV6 */
#endif /* MULTICASTSOCKETV6_H */
