/* SockException.h
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

#ifndef SOCKEXCEPTION_H
#define SOCKEXCEPTION_H

#include <sockclasslib.h>
#include <BaseException.h>
#include <iostream>
using namespace std;

#ifdef _MSC_VER
#include <winbase.h>
#endif /* _MSC_VER */

/// \file

#ifndef _WITHOUT_SOCK_EXCEPTIONS
/// \addtogroup EXCEPT_GROUP
/// \{

/// Throw socket exception.
#define SOCK_EXCEPT_THROW(x, s) \
    SockException SOCK_EXCEPT(EXCEPTION_PARAMS, x, s); \
    throw(SOCK_EXCEPT)

/// Catch and re-throw socket exception.
#define SOCK_EXCEPT_CATCH_AND_RAISE(x) \
    catch(SockException&) { \
        throw; \
    }

/// Catch socket exception without report. Opens catch block.
#define SOCK_EXCEPT_CATCH_BEGIN_NOREP \
    catch(SockException& exc) { \
        exc.getErrCode(); // To eliminate warning about unreferenced variable

/// Catch socket exception with report. Opens catch block.
#define SOCK_EXCEPT_CATCH_BEGIN(x) \
    catch(SockException& exc) { \
        if(x) { \
            x << "******" << std::endl; \
            x << exc.what(); \
            x << "******" << std::endl << std::endl << std::flush; \
        }

/// Catch all aka catch(...)
#define SOCK_EXCEPT_CATCH_ALL(x) \
    } \
    catch(...) { \
        if(x) { \
            x << "Unknown exception" << std::endl << std::flush; \
        }

/// Close catch block.
#define SOCK_EXCEPT_CATCH_END \
    }


/// Try
#define SOCK_TRY try
/// \}

#else /* _WITHOUT_SOCK_EXCEPTIONS */

#define SOCK_EXCEPT_THROW(x)

#define SOCK_EXCEPT_CATCH_AND_RAISE(x)

#define SOCK_EXCEPT_CATCH_BEGIN(x) \
    {

#define SOCK_EXCEPT_CATCH_BEGIN_NOREP \
    {

#define SOCK_EXCEPT_CATCH_ALL(x) \
    } { \

#define SOCK_EXCEPT_CATCH_END \
    }

#define SOCK_TRY

#endif /* _WITHOUT_SOCK_EXCEPTIONS */


///
/// \class SockException
/// Socket class library exception.
/// 
///

class SOCKLIB_API SockException : public BaseException
{
public:
    /// Constructor
    /// \details Constructs exception object with filename and line number.
    /// \param FileName : name of source file.
    /// \param LineNum : number of code line.
    /// \param errcode : error numeric code.
    SockException(const char* FileName, int LineNum, int errcode, SOCKET Sock);
    /// Constructor
    /// \details Constructs exception object with filename, line number and text message
    /// \param FileName : name of source file.
    /// \param LineNum : number of code line.
    /// \param Message : user message.
    SockException(const char* FileName, int LineNum, const char* Message, SOCKET Sock);
    /// Destructor
    /// \details Intentionally empty
    virtual ~SockException()
    {
    }

    /// Reports exception's properties.
    /// \return String buffer with report text.
    virtual const char* what(); // Overload

    /// Format error message.
    char* formatMessage();
    /// Format error message.
    static bool formatMessage(char(*MsgBuf)[256]);

    /// Delivers error code
    /// \return error cod of the exception cause.
    int getErrCode()
    {
        return m_ErrCode;
    }

    /// Delivers error message.
    /// \return message buffer.
    char* getMessage()
    {
        return m_Message;
    }

    /// Delivers errno
    /// \return errno
    static int getErrNum()
    {
        return WSAGetLastError();
    }

    /// Sets error number.
    /// \param Num : error number.
    static void setErrNum(int Num = 0)
    {
        WSASetLastError(Num);
    }

    ///
    /// \brief getSock
    /// \return - file descriptor of the socket made exception.
    ///
    SOCKET getSock()
    {
        return m_Sock;
    }

protected:
    int m_ErrCode; ///< Error code.
    char lpMsgBuf[256]; ///< Error message buffer.
    char m_Message[256]; ///< UserMessage buffer.
    SOCKET m_Sock; ///< Socket that coused exception.
};

#endif

