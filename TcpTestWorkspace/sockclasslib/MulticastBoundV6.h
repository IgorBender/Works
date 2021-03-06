/* MulticastBoundV6.h
 *
 * Copyright 2000 Igor Bender
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef MULTICASTBOUNDV6_H_
#define MULTICASTBOUNDV6_H_

#ifdef _WIN32
#pragma warning ( disable : 4251 )
#endif

#ifndef _NO_IPV6

#include <MulticastSocketV6.h>
#include <BoundSocketV6.h>

#include <list>

bool operator==(const in6_addr& left, const in6_addr& right)
{
	return (!memcmp(&left, &right, sizeof(in6_addr)));
}

typedef std::list < in6_addr > GroupsListType;

/// IPv4 socket capable to receive multicast datagrams.
class SOCKLIB_API MulticastBoundV6 : public BoundSocketV6, public MulticastSocketV6
{
public:
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Constructor.
    /// \param Port : port number for bind in network byte order.
    /// \param Address : address for bind in network byte order.
    /// \throw SockException.
    MulticastBoundV6(short Port, in6_addr Address = in6addr_any) throw (SockException);
    /// Constructor.
    /// \param Port : port number for bind in network byte order.
    /// \param Address : address for bind in textual notation.
    /// \throw SockException.
    MulticastBoundV6(short Port, const char* Address) throw (SockException);
#else
    MulticastBoundV6(short Port, in6_addr Address = in6addr_any);
    MulticastBoundV6(short Port, const char* Address);
#endif

#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Join multicast group.
    /// Param Group : multicast group address in network byte order.
    /// \throw SockException.
    void joinGroup(in6_addr Group) throw (SockException);
    /// Leave multicast group.
    /// Param Group : multicast group address in network byte order.
    /// \throw SockException.
    void leaveGroup(in6_addr Group) throw (SockException);
    /// Leave all multicast groups previously joined.
    /// \throw SockException.
    void leaveAllGroups() throw (SockException);
#else
    int joinGroup(in6_addr Group);
    int leaveGroup(in6_addr Group);
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
    in6_addr Addr; ///< Bound address.

private:
    /// Copy constructor.
    /// Makes the class uncopyable.
    MulticastBoundV6(MulticastBoundV6& s);
    /// Operator assign.
    /// Makes the class uncopyable.
    MulticastBoundV6& operator=(MulticastBoundV6&);
};

#endif /* _NO_IPV6 */
#endif /* MULTICASTBOUNDV6_H_ */
