/*
 *  DatagramBoundUds.h
 *
 *  Created on: 2023-02-18
 *  Author: Igor Bender
 *
 */
/* DatagramBoundUds.h
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

#ifndef DATAGRAMBOUNDUDS_H
#define DATAGRAMBOUNDUDS_H

#include "DatagramSocketUds.h"
#include "BoundSocketUds.h"

/// UDP socket bound to specified path (aka UDP server).
class SOCKLIB_API DatagramBoundUds : virtual public DatagramSocketUds, virtual public BoundSocketUds
{
public:
    /// Constructor.
    /// \param Path : socket path.
    /// \throw SockException.
    DatagramBoundUds(const std::string& Path);

    /// Receive datagram.
    /// Instantiotion of uure virtual method.
    /// \param Buffer : buffer for received data .
    /// \param Length : size of buffer.
    /// \param Flags : flags, default value - 0.
    /// \throw SockException
    virtual int receiveFrom(void* Buffer, size_t Length, int Flags = 0)
    {
        return DatagramSocketUds::receiveFrom(Buffer, Length, Flags);
    }

private:
    /// Copy constructor.
    /// Makes the class uncopyable.
    DatagramBoundUds(DatagramBoundUds& d);
    /// Operator assign.
    /// Makes the class uncopyable.
    DatagramBoundUds& operator=(DatagramBoundUds& d);
};

#endif // DATAGRAMBOUNDUDS_H
