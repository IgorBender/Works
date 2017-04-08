/* InternetSocket.h
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

#ifndef _INTERNETSOCKET_H
#define _INTERNETSOCKET_H

#ifdef _WIN32
#pragma warning( disable : 4290 )
#pragma warning( disable : 4250 )

#endif /* _WIN32 */

#include <SocketClass.h>

/// Bbase abstract class.
/// Is base for all kinds of IP sockets,
/// delivers functionality of manipulating IP level socket options.
class SOCKLIB_API InternetSocket : public SocketClass
{
public:
    /// Constructor.
    /// Default constructor.
    InternetSocket(SockDomain domain = NO_DOMAIN, SockType type = NO_TYPE, int protocol = 0) :
    	SocketClass(domain, type, protocol)
    {}
    /// Destructor
    virtual ~InternetSocket()
    {}

//#ifndef _WITHOUT_SOCK_EXCEPTIONS
//    /// Set IPv4 level option.
//    /// \param Opt : option type.
//    /// \param Value : option value.
//    /// \param OptLen : option value length.
//    /// \throw SockException
//    void setIpLevelOpt(int Opt, const char* Value, int OptLen) throw (SockException);
//    /// Deliver IPv4 level option.
//    /// \param Opt : option type.
//    /// \param Value : option value.
//    /// \param OptLen : option value length.
//    /// \throw SockException
//    void getIpLevelOpt(int Opt, char* Value, int* OptLen) throw (SockException);
//#else
//    int setIpLevelOpt(int Opt, const char* Value, int OptLen);
//    int getIpLevelOpt(int Opt, char* Value, int* OptLen);
//#endif

protected:
    /// Constructor.
    /// Constructs socket on given socket file descriptor/handle.
    /// \param sock : socket file descriptor/handle.
    /// \param domain : socket domain (IPv4 or IPv6)
    /// \param type : socket type.
    /// \param protocol : socket protocol.
    InternetSocket(int sock, SockDomain domain, SockType type = NO_TYPE, int protocol = 0) :
    	SocketClass(sock, domain, type, protocol)
    {}
};

#endif

