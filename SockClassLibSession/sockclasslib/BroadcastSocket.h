/* BroadcastSocket.h
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

#ifndef BROADCASTSOCKET_H
#define BROADCASTSOCKET_H

#include <DatagramSocket.h>

/// Broadcast socket class.
class SOCKLIB_API BroadcastSocket : public DatagramSocket
{
public:
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Constructor.
    /// Default constructor.
    /// \throw SockException.
    BroadcastSocket();
#else
    BroadcastSocket();
#endif

#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Send broadcast.
    /// \param Port : destination port in network byte order.
    /// \param Buffer : data to send.
    /// \param Length : amount of the data.
    /// \param Flags : flags, default value - 0.
    /// \throw SockException.
    int sendBroadcast(uint16_t Port, const void* Buffer, size_t Length,
                      int Flags = 0)
#else

    int sendBroadcast(uint16_t Port, const void* Buffer, size_t Length,
                      int Flags = 0)
#endif
    {
        setDestination(INADDR_BROADCAST, Port);
        return sendTo(Buffer, Length, Flags);
    }
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Send subnet broadcast.
    /// \param Addr : subnet address in network byte order.
    /// \param Port : destination port in network byte order.
    /// \param Buffer : data to send.
    /// \param Length : amount of the data.
    /// \param Flags : flags, default value - 0.
    /// \throw SockException.
    int sendSubnetBroadcast(in_addr_t Addr, uint16_t Port, const void* Buffer, size_t Length,
                            int Flags = 0)
#else

    int sendSubnetBroadcast(in_addr_t Addr, uint16_t Port, const void* Buffer, size_t Length,
                            int Flags = 0)
#endif
    {
        setDestination(Addr, Port);
        return sendTo(Buffer, Length, Flags);
    }
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

protected:

private:
    /// Copy constructor.
    /// Makes the class uncopyable.
    BroadcastSocket(BroadcastSocket& b);
    /// Operator assign.
    /// Makes the class uncopyable.
    BroadcastSocket& operator=(BroadcastSocket& b);
};

#endif

