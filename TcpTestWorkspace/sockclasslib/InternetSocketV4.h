/* InternetSocketV4.h
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

#ifndef INTERNETSOCKETV4_H_
#define INTERNETSOCKETV4_H_

#include <InternetSocket.h>

/// IPv4 base abstract class.
/// Is base for all kinds of IPv4 sockets.
class SOCKLIB_API InternetSocketV4 : virtual public InternetSocket
{
public:
    /// Constructor.
    /// Default constructor.
    InternetSocketV4(SockType type = NO_TYPE, int protocol = 0) :
    	InternetSocket(INTERNET_DOMAIN, type, protocol)
    {
        m_Domain = INTERNET_DOMAIN;
        m_Type = type;
        m_Protocol = protocol;
    }

	virtual ~InternetSocketV4()
	{}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Set IPv4 level option.
    /// \param Opt : option type.
    /// \param Value : option value.
    /// \param OptLen : option value length.
    /// \throw SockException
    void setIpLevelOpt(int Opt, const char* Value, int OptLen) throw (SockException);
    /// Deliver IPv4 level option.
    /// \param Opt : option type.
    /// \param Value : option value.
    /// \param OptLen : option value length.
    /// \throw SockException
    void getIpLevelOpt(int Opt, char* Value, int* OptLen) throw (SockException);
#else
    int setIpLevelOpt(int Opt, const char* Value, int OptLen);
    int getIpLevelOpt(int Opt, char* Value, int* OptLen);
#endif

protected:
    /// Constructor.
    /// Constructs socket on given socket file descriptor/handle.
    /// \param sock : socket file descriptor/handle.
    /// \param type : socket type.
    /// \param protocol : socket protocol.
    InternetSocketV4(int sock, SockType type = NO_TYPE, int protocol = 0) :
    	InternetSocket(sock, INTERNET_DOMAIN, type, protocol)
    {
        m_Sock = sock;
        m_Domain = INTERNET_DOMAIN;
        m_Type = type;
        m_Protocol = protocol;
    }
};

#endif /* INTERNETSOCKETV4_H_ */
