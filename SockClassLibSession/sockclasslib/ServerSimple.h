/* ServerSimple.h
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

#ifndef _SIMPLESERVER_H
#define _SIMPLESERVER_H

#include <StreamBoundV4.h>

/// TCP server socket.
class SOCKLIB_API ServerSimple : public StreamBoundV4
{
public:
    /// Constructor.
    /// \param Port : port number for bind in network byte order.
    /// \param Address : address for bind in network byte order.
    /// \param LateBind : do bind now or later according to bind method call.
    /// \throw SockException.
    ServerSimple(short Port, in_addr_t Address = INADDR_ANY,
                 bool LateBind = false) : StreamBoundV4(Port, Address, LateBind)
    {
    }
    /// Constructor.
    /// \param Port : port number for bind in network byte order.
    /// \param Address : IPv4 address for bind in decimal dot notation.
    /// \param LateBind : do bind now or later according to bind method call.
    /// \throw SockException.
    ServerSimple(short Port, const char* Address, bool LateBind = false) : StreamBoundV4(Port, Address, LateBind)
    {
    }

#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Listen for input connections.
    /// \param Backlog : number of simultaneously held connection requests.
    /// \throw SockException.
    virtual void listen(int Backlog = 5);
    /// Accept connection request.
    /// \return StreamSocketV4 pointer representing local connection endpoint.
    /// \throw SockException.
    virtual StreamSocketV4* accept();
#else

    virtual int listen(int Backlog = 5);
    virtual StreamSocketV4* accept();
#endif

private:
    /// Copy constructor.
    /// Makes the class uncopyable.
    ServerSimple(ServerSimple& s);
    /// Operator assign.
    /// Makes the class uncopyable.
    ServerSimple& operator=(ServerSimple& s);
};

#endif /* _SIMPLESERVER_H */

