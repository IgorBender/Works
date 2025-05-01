/* MulticastBoundV6.h
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

#ifndef MULTICASTBOUNDV6_H
#define MULTICASTBOUNDV6_H

#ifdef _MSC_VER
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
    /// Constructor.
    /// \param Port : port number for bind in network byte order.
    /// \param Address : address for bind in network byte order.
    /// \throw SockException.
    MulticastBoundV6(uint16_t Port, in6_addr Address = in6addr_any);
    /// Constructor.
    /// \param Port : port number for bind in network byte order.
    /// \param Address : address for bind in textual notation.
    /// \throw SockException.
    MulticastBoundV6(uint16_t Port, const char* Address);

#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Join multicast group.
    /// Param Group : multicast group address in network byte order.
    /// \throw SockException.
    void joinGroup(in6_addr Group);
    /// Leave multicast group.
    /// Param Group : multicast group address in network byte order.
    /// \throw SockException.
    void leaveGroup(in6_addr Group);
#else
    int joinGroup(in6_addr Group);
    int leaveGroup(in6_addr Group);
#endif
    /// Leave all multicast groups previously joined.
    /// \throw SockException.
    void leaveAllGroups();
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
#endif /* MULTICASTBOUNDV6_H */
