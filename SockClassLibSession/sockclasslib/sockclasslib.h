/* sockclasslib.h
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

/*! \mainpage Socket Class Library
 * 
 * \author Igor Bender.
 * 
 * \section intro_sec Introduction
 *
 * 
 * <b>Socket Class Library</b> is designed in aim to be a cross-platform socket class library. The <b>SCL</b> is
 * successfully ported on Linux, Solaris, QNX, VxWorks and Windows platforms. The
 * <b>SCL</b> may be used as a basis for further class libraries providing
 * extended services. The SCL is still under construction and this
 * document may be not fully updated.
 * <br>
 * The <b>SCL</b> provides classes for datagram oriented unreliable
 * communication (UDP) and stream oriented reliable communication (TCP).
 *
 * 
 */

#ifndef SOCKLIB_H
#define SOCKLIB_H

#ifdef _MSC_VER

#ifdef SOCKCLASSLIB_EXPORTS
#define SOCKLIB_API __declspec(dllexport)
#else
#define SOCKLIB_API __declspec(dllimport)
#endif

#define IP_DEFAULT_MULTICAST_TTL 1
#define INADDR_ALLHOSTS_GROUP 0xe0000001 // 224.0.0.1

#include <winsock2.h>
#ifndef _NO_IPV6
#include <Ws2ipdef.h>
#endif /* _NO_IPV6 */
#include <WS2tcpip.h>
#include <stdint.h>

#define SENDCONNRESET WSAECONNRESET
#define RECVCONNRESET WSAECONNRESET

typedef int size_type;
typedef int socklen_type;
typedef uint32_t in_addr_t;
//typedef SOCKADDR_IN6 sockaddr_in6;

extern HANDLE hLibMod;

#else /* _MSC_VER */

typedef int SOCKET;

#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/socket.h>

#if (__linux__ || __cygwin__)
//#include <arpa/inet.h>
//typedef unsigned in_addr_t;
#endif

#if (__sun || __linux__)
#ifndef __linux__
#ifndef INADDR_NONE
#define INADDR_NONE 0xFFFFFFFF
#endif
#endif
typedef socklen_t socklen_type;
#else
typedef int socklen_type;
#endif

#ifdef __VXWORKS__
#include <sockLib.h>
#include <selectLib.h>
typedef unsigned in_addr_t;
#endif

#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#define SENDCONNRESET EPIPE
#define RECVCONNRESET ECONNRESET
#define WSAECONNRESET ECONNRESET
#define WSAEISCONN EISCONN
#define WSAEWOULDBLOCK EWOULDBLOCK
#define WSAEINPROGRESS EINPROGRESS
#define WSAEALREADY EALREADY
#define WSAGetLastError() errno
#define WSASetLastError(x) errno = x

inline int closesocket(int sock)
{
    return ::close(sock);
}

typedef size_t size_type;

#define SOCKLIB_API // Intentionally empty

#endif /* _MSC_VER */


namespace SockClassLib
{
/// \file 
///
/// \defgroup LIB_GROUP Library Auxiliaries.
/// 
/// \{
    /// Structure for library versions.
    struct VersionTriple
    {
        int Major; ///< Major version number.
        int Minor; ///< Minor version number.
        int SubMinor; ///< Minor subversion number.
    };

    /// Get socket class library version.
    /// \param Versions : VersionTriple structure to be filled with versions.
    inline void SOCKLIB_API getVersion(VersionTriple& Versions)
    {
        Versions.Major = 4;
        Versions.Minor = 0;
        Versions.SubMinor = 0;
    }
/// \}
}

#endif /* _SOCKLIB_H */

