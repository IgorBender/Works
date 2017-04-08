/* ServerReuse.cpp
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

#include <ServerReuse.h>

#ifndef _WITHOUT_SOCK_EXCEPTIONS
ServerReuse::ServerReuse(short Port, in_addr_t Address) throw(SockException) :
        ServerSimple(Port, Address, true)
#else
ServerReuse::ServerReuse(short Port, in_addr_t Address) :
        ServerSimple(Port, Address, true)
#endif
{
    struct linger Linger;
    Linger.l_onoff = 1;
    Linger.l_linger = 0;
    setSockLevelOpt(SO_LINGER, reinterpret_cast < const char* > (&Linger),
                    sizeof(struct linger));
    int Opt = 1;
    setSockLevelOpt(SO_REUSEADDR, reinterpret_cast < const char* > (&Opt),
                    sizeof Opt);

    if(!isBound())
    {
        setEndPoint(Address, Port);
        bind();
    }
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
ServerReuse::ServerReuse(short Port, const char* Address)
    throw(SockException) : ServerSimple(Port, Address, true)
#else
ServerReuse::ServerReuse(short Port, const char* Address) : ServerSimple(Port, Address, true)
#endif
{
    struct linger Linger;
    Linger.l_onoff = 1;
    Linger.l_linger = 0;
    setSockLevelOpt(SO_LINGER, reinterpret_cast < const char* > (&Linger),
                    sizeof(struct linger));
    int Opt = 1;
    setSockLevelOpt(SO_REUSEADDR, reinterpret_cast < const char* > (&Opt),
                    sizeof Opt);

    if(!isBound())
    {
        setEndPoint(Address, Port);
        bind();
    }
}

