/* StreamBoundUds.h
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

#ifndef STREAMBOUNDUDS_H
#define STREAMBOUNDUDS_H

#include "StreamSocketUds.h"
#include "BoundSocketUds.h"

/// TCP socket bound to specified socket path.
class SOCKLIB_API StreamBoundUds : virtual public BoundSocketUds, virtual public StreamSocketUds
{
public:
public:
    /// Constructor.
    /// \param Path : Socket path for bind.
    /// \param LateBind : do bind now or later according to bind method call.
    /// \throw SockException.
    StreamBoundUds(const std::string& Path, bool LateBind = false);

private:
    /// Copy constructor.
    /// Makes the class uncopyable.
    StreamBoundUds(StreamBoundUds& s);
    /// Operator assign.
    /// Makes the class uncopyable.
    StreamBoundUds& operator=(StreamBoundUds& s);
};

#endif // STREAMBOUNDUDS_H
