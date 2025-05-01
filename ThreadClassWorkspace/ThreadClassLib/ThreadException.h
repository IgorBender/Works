/* ThreadException.h
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

#ifndef THREADEXCEPTION_H
#define THREADEXCEPTION_H

#include "ThreadClassLib.h"

#ifdef _MSC_VER
#pragma warning(disable : 4786)
#pragma warning(disable : 4251)
#endif

#include <string.h>

#ifndef GCC_VERSION
#define GCC_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
#endif /* GCC_VERSION */

#include <vector>
#if (GCC_VERSION >= 3001 || defined _MSC_VER)
#include <sstream>
#define thrrepstream std::stringstream
#else
#include <strstream>
#include <string>
#define thrrepstream std::strstream
#endif

#if (__QNX__ || __VXWORKS__)
#include <string>
#endif

#ifdef _MSC_VER
#include <windows.h>
#include <winbase.h>
#endif /* _MSC_VER */

/// \addtogroup EXCEPT_GROUP
/// \{

/// Exception parameters includes filename and line number.
#define THREAD_EXCEPTION_PARAMS __FILE__, __LINE__

/// Throw exception based on filename and line number.
#define THREAD_EXCEPT_THROW(x) \
    ThreadClassLib::ThreadException THREAD_EXCEPT(THREAD_EXCEPTION_PARAMS, x); \
    throw(THREAD_EXCEPT)

/// Catch and re-throw an exception.
#define THREAD_EXCEPT_CATCH_AND_RAISE(x) \
    catch(ThreadClassLib::ThreadException&) { \
        throw; \
    }

/// Begin catch block with no reporter.
#define THREAD_EXCEPT_CATCH_BEGIN_NOREP \
	catch(ThreadClassLib::ThreadException& exc) { \
		exc; // To eliminate warning about unreferenced variable

/// Begin catch block with reporting to ostream.
#define THREAD_EXCEPT_CATCH_BEGIN(x) \
    catch(ThreadClassLib::ThreadException& exc) { \
        if(x) { \
            x << "******" << std::endl; \
            x << exc.what(); \
            x << "******" << std::endl << std::endl << std::flush; \
        }

/// Catch all - aka catch(...).
#define THREAD_EXCEPT_CATCH_ALL(x) \
    } \
    catch(...) { \
        if(x) { \
            x << "Unknown exception" << std::endl << std::flush; \
        }

/// End catch block.
#define THREAD_EXCEPT_CATCH_END \
    }

/// Begin try block.
#define THREAD_TRY try

/// \}

namespace ThreadClassLib
{
///
/// \class ThreadException
/// Thread class library exception.
///
class THREADCLASSLIB_API ThreadException
{
public:
    /// Constructor
    /// @details Constructs exception object with filename and line number.
    /// @param FileName : name of source file.
    /// @param LineNum : number of code line.
    /// @param errcode : error numeric code.
    ThreadException(const char* FileName, int LineNum, int errcode);
    /// Constructor
    /// @details Constructs exception object with filename, line number and text message
    /// @param FileName : name of source file.
    /// @param LineNum : number of code line.
    /// @param Message : user message.
    ThreadException(const char* FileName, int LineNum, const char* Message);
    /// Constructor.
    /// @details Copy constructor
    ThreadException(const ThreadException& exp)
    {
        ::strcpy(m_FileName, exp.m_FileName);
        m_LineNum = exp.m_LineNum;
        ::strcpy(m_Message, exp.m_Message);
        m_ErrCode = exp.m_ErrCode;
    }
    /// Destructor
    /// @details Intentionally empty
    virtual ~ThreadException()
    {
    }

    /// Reports exception's properties.
    /// @return String buffer with report text.
    virtual const char* what(); // Overload

    /// Delivers error code
    /// @return error cod of the exception cause.
    int getErrCode()
    {
        return m_ErrCode;
    }

    /// Delivers error message.
    /// @return message buffer.
    char* getMessage()
    {
        return m_Message;
    }

    /// Delivers errno
    /// @return errno
    int getErrNum()
    {
        return m_ErrCode;
    }

    /// Sets error number.
    /// @param Num : error number.
    void setErrNum(int Num = 0)
    {
        m_ErrCode = Num;
    }

protected:
    char m_FileName[256]; ///< Buffer for filename of the exception source.
    int m_LineNum; ///< Line number of the exception source.
    std::string repstr; ///< String for the exception report will be build to.
    int m_ErrCode; ///< Error code.
    char lpMsgBuf[256]; ///< Error message buffer.
    char m_Message[256]; ///< UserMessage buffer.
};
} // end of ThreadClassLib namespace

#endif // THREADEXCEPTION_H
