/*
 *  ServerSimpleUds.h
 *
 *  Created on: 2023-02-18
 *  Author: Igor Bender
 *
 */
/* ServerSimpleUds.h
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

#ifndef SERVERSIMPLEUDS_H
#define SERVERSIMPLEUDS_H

#include "StreamBoundUds.h"

/// TCP server socket.
class SOCKLIB_API ServerSimpleUds : public StreamBoundUds
{
public:
    /// Constructor.
    /// \param Path : socket path for bind.
    /// \param LateBind : do bind now or later according to bind method call.
    /// \throw SockException.
    ServerSimpleUds(const std::string& Path,
                 bool LateBind = false) : StreamBoundUds(Path, LateBind)
    {
    }
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Listen for input connections.
    /// \param Backlog : number of simultaneously held connection requests.
    /// \throw SockException.
    virtual void listen(int Backlog = 5);
    /// Accept connection request.
    /// \return StreamSocketV4 pointer representing local connection endpoint.
    /// \throw SockException.
    virtual StreamSocketUds* accept();
#else

    virtual int listen(int Backlog = 5);
    virtual StreamSocketUds* accept();
#endif

private:
    /// Copy constructor.
    /// Makes the class uncopyable.
    ServerSimpleUds(ServerSimpleUds& s);
    /// Operator assign.
    /// Makes the class uncopyable.
    ServerSimpleUds& operator=(ServerSimpleUds& s);

};

#endif // SERVERSIMPLEUDS_H
