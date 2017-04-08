/* StreamBoundV6.cpp
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

#ifndef _NO_IPV6

#include <StreamBoundV6.h>

#ifndef _WITHOUT_SOCK_EXCEPTIONS
StreamBoundV6::StreamBoundV6(short Port, in6_addr Address, bool LateBind) throw(SockException)
#else
StreamBoundV6::StreamBoundV6(short Port, in6_addr Address, bool LateBind)
#endif
{
    if(!isBound())
    {
        setEndPoint(Address, Port);
        if(!LateBind)
        {
            bind();
        }
    }
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
StreamBoundV6::StreamBoundV6(short Port, const char* Address, bool LateBind) throw(SockException)
#else
StreamBoundV6::StreamBoundV6(short Port, const char* Address, bool LateBind)
#endif
{
    if(!isBound())
    {
        setEndPoint(Address, Port);
        if(!LateBind)
        {
            bind();
        }
    }
}

#endif /* _NO_IPV6 */
