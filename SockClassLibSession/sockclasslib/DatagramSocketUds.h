/* DatagramSocketUds.h
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

#ifndef DATAGRAMSOCKETUDS_H
#define DATAGRAMSOCKETUDS_H

#include "ConnectedSocketUds.h"

/// Class for UDP sockets.
class SOCKLIB_API DatagramSocketUds : virtual public ConnectedSocketUds
{
public:
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    ///  Constructor.
    /// Default constructor.
    /// \param protocol : socket protocol, default value - 0.
    /// \throw SockException
    DatagramSocketUds(int protocol = 0);
#else
    DatagramSocketUds(int protocol = 0);
#endif

    ///  Destructor.
    virtual ~DatagramSocketUds();

    /// Delivers source of the last received datagram.
    /// \return sockaddr_in structure field with source of the last received datagram.
    sockaddr_un& getSource()
    {
        return m_Source;
    }
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Send datagram to defined destination.
    /// Destination should be define with "setDestination" method.
    /// \param Buffer : data for send.
    /// \param Length : size of data to send.
    /// \param Flags : flags, default value - 0.
    /// \throw SockException
    int sendTo(const void* Buffer, size_t Length, int Flags = 0);

    /// Send datagram to the source of last received datagram.
    /// \param Buffer : data for send.
    /// \param Length : size of data to send.
    /// \param Flags : flags, default value - 0.
    /// \throw SockException
    int sendToLast(const void* Buffer, size_t Length, int Flags = 0)
    {
        setDestination(m_Source);
        return sendTo(Buffer, Length, Flags);
    }
    /// Receive datagram.
    /// Pure virtual method to make the class abstract. Nobody could send
    /// to unbound Unix domain datagram socket as its destination is unknown.
    /// Receive functionality is saved for derived class.
    /// \param Buffer : buffer for received data .
    /// \param Length : size of buffer.
    /// \param Flags : flags, default value - 0.
    /// \throw SockException
    virtual int receiveFrom(void* Buffer, size_t Length, int Flags = 0) = 0;
#else
    int sendTo(const void* Buffer, size_t Length, int Flags = 0);

    int sendToLast(const void* Buffer, size_t Length, int Flags = 0)
    {
        setDestination(m_Source);
        return sendTo(Buffer, Length, Flags);
    }
    virtual int receiveFrom(void* Buffer, size_t Length, int Flags = 0) = 0;

    bool isOk()
    {
        return IsOk;
    }
#endif

protected:
    /// Close socket.
    virtual void close()
    {
        closesocket(m_Sock);
    }

    sockaddr_un m_Source; ///< Properties of the endpoint, that last received datagram was send from.
#ifdef _WITHOUT_SOCK_EXCEPTIONS
    bool IsOk;
#endif

private:
    /// Copy constructor.
    /// Makes the class uncopyable.
    DatagramSocketUds(DatagramSocketUds& d);
    /// Operator assign.
    /// Makes the class uncopyable.
    DatagramSocketUds& operator=(DatagramSocketUds& d);
};

#endif // DATAGRAMSOCKETUDS_H
