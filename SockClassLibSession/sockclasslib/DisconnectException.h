/* DisconnectException.h
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

#ifndef _DISCONNECTEXCEPTION_H
#define _DISCONNECTEXCEPTION_H

#include <SockException.h>

/// \file

#ifndef _WITHOUT_SOCK_EXCEPTIONS
/// \addtogroup EXCEPT_GROUP
/// \{

/// Throw disconnect exception.
#define DISCONN_EXCEPT_THROW(x) \
    if(x) { \
        DisconnectException EXCEPT(EXCEPTION_PARAMS, x); \
        throw(EXCEPT); \
    } else { \
        DisconnectException EXCEPT(EXCEPTION_PARAMS); \
        throw(EXCEPT); \
    } \

/// Catch and re-throw disconnect exception.
#define DISCONN_EXCEPT_CATCH_AND_RAISE(x) \
    catch(DisconnectException&) { \
        throw; \
    }

/// Catch disconnect exception with report. Opens catch block.
#define DISCONN_EXCEPT_CATCH_BEGIN(x) \
    catch(DisconnectException& exc) { \
        if(x) { \
            x << "******" << std::endl; \
            x << exc.what(); \
            x << "******" << std::endl << std::endl << std::flush; \
        }

/// Catch disconnect exception without report. Opens catch block.
#define DISCONN_EXCEPT_CATCH_BEGIN_NOREP \
	catch(DisconnectException& exc) { \
	exc.getErrCode(); // To eliminate warning about unreferenced variable


/// Catch all aka catch(...)
#define DISCONN_EXCEPT_CATCH_ALL(x) \
    } \
    catch(...) { \
        if(x) { \
            x << "Unknown exception" << std::endl << std::flush; \
        }

/// Close catch block.
#define DISCONN_EXCEPT_CATCH_END \
    }

/// Try
#define DISCONN_TRY try

/// \}

#else /* _WITHOUT_SOCK_EXCEPTIONS */

#define DISCONN_EXCEPT_THROW(x)

#define DISCONN_EXCEPT_CATCH_AND_RAISE(x)

#define DISCONN_EXCEPT_CATCH_BEGIN(x) \
    {

#define DISCONN_EXCEPT_CATCH_BEGIN_NOREP \
    {

#define DISCONN_EXCEPT_CATCH_ALL(x) \
    } { \

#define DISCONN_EXCEPT_CATCH_END \
    }

#define DISCONN_TRY

#endif /* _WITHOUT_SOCK_EXCEPTIONS */

///
/// \class DisconnectException
/// Stream socket disconnection exception.
class SOCKLIB_API DisconnectException : public SockException {
public:
    /// Constructor
    /// \details Constructs exception object with filename and line number.
    /// \param FileName : name of source file.
    /// \param LineNum : number of code line.
    /// \param errcode : error numeric code, default value CONNRESET.
    DisconnectException(const char* FileName, int LineNum,
                        int errcode = WSAECONNRESET) : SockException(FileName, LineNum, errcode)
    {
    }
    /// Constructor
    /// \details Constructs exception object with filename, line number and text message
    /// \param FileName : name of source file.
    /// \param LineNum : number of code line.
    /// \param Msg : user message.
    DisconnectException(const char* FileName, int LineNum,
                        const char* Msg) : SockException(FileName, LineNum, Msg)
    {
    }
};

#endif

