/* DatagramBoundV6.cpp
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

#include <DatagramBoundV6.h>

DatagramBoundV6::DatagramBoundV6(uint16_t Port, in6_addr Address)
{
    if(!isBound())
    {
        setEndPoint(Address, Port);
        bind();
    }
}


DatagramBoundV6::DatagramBoundV6(uint16_t Port, const char* Address)
{
    if(!isBound())
    {
        setEndPoint(Address, Port);
        bind();
    }
}

#endif /* _NO_IPV6 */
