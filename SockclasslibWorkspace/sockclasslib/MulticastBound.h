/* MulticastBound.h
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

#ifndef _MULTICASTBOUND_H
#define _MULTICASTBOUND_H

#ifdef _WIN32
#pragma warning ( disable : 4251 )
#endif

#include <MulticastSocket.h>
#include <BoundSocketV4.h>

#include <list>

typedef std::list < in_addr_t > GroupsListType;

/// IPv4 socket capable to receive multicast datagrams.
class SOCKLIB_API MulticastBound : public BoundSocketV4, public MulticastSocket
{
public:
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Constructor.
    /// \param Port : port number for bind in network byte order.
    /// \param Address : address for bind in network byte order.
    /// \throw SockException.
    MulticastBound(short Port, in_addr_t Address = INADDR_ANY) throw (SockException);
    /// Constructor.
    /// \param Port : port number for bind in network byte order.
    /// \param Address : address for bind in decimal dot notation.
    /// \throw SockException.
    MulticastBound(short Port, const char* Address) throw (SockException);
#else
    MulticastBound(short Port, in_addr_t Address = INADDR_ANY);
    MulticastBound(short Port, const char* Address);
#endif

#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Join multicast group.
    /// Param Group : multicast group address in network byte order.
    /// \throw SockException.
    void joinGroup(in_addr_t Group) throw (SockException);
    /// Leave multicast group.
    /// Param Group : multicast group address in network byte order.
    /// \throw SockException.
    void leaveGroup(in_addr_t Group) throw (SockException);
    /// Leave all multicast groups previously joined.
    /// \throw SockException.
    void leaveAllGroups() throw (SockException);
#else
    int joinGroup(in_addr_t Group);
    int leaveGroup(in_addr_t Group);
    void leaveAllGroups();
#endif
    /// Get list of joined multicast groups.
    /// \return group list pointer.
    GroupsListType* getGroups()
    {
        return &GroupsList;
    }

protected:
    GroupsListType GroupsList; ///< Joined multicast groups list.
    in_addr_t Addr; ///< Bound address.

private:
    /// Copy constructor.
    /// Makes the class uncopyable.
    MulticastBound(MulticastBound& s);
    /// Operator assign.
    /// Makes the class uncopyable.
    MulticastBound& operator=(MulticastBound&);
};

#endif /* _MULTICASTBOUND_H */
