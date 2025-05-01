/* ClientBoundUds.h
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

#ifndef CLIENTBOUNDUDS_H
#define CLIENTBOUNDUDS_H

#include <ClientSimpleUds.h>
#include <BoundSocketUds.h>

/// Client socket bound to specified path.
class SOCKLIB_API ClientBoundUds : public ClientSimpleUds, public BoundSocketUds
{
public:
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    /// Constructor.
    /// \param Port : port number for bind in network byte order.
    /// \param Path : path bind.
    /// \param LateBind : do bind now or later according to bind method call.
    /// \throw SockException.
    ClientBoundUds(const std::string& Path, bool LateBind = false)
    {
        if(!isBound())
        {
            setEndPoint(Path);
            if(!LateBind)
            {
                bind();
            }
        }
    }

#else
    ClientBound(std::string Path, bool LateBind = false)
    {
        if(!isBound())
        {
            setEndPoint(Path);
            if(!LateBind)
            {
                bind();
            }
        }
    }

#endif

private:
    /// Copy constructor.
    /// Makes the class uncopyable.
    ClientBoundUds(ClientBoundUds& s);
    /// Operator assign.
    /// Makes the class uncopyable.
    ClientBoundUds& operator= (ClientBoundUds& s);
};

#endif // CLIENTBOUNDUDS_H
