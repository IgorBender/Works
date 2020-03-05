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

#ifdef _TRACE_THREAD_EXCEPTIONS
#include "PThreadTraceException.h"
#else
#include "PThreadBaseException.h"
#endif
#include <iostream>
using namespace std;

#ifdef _WIN32
#include <windows.h>
#include <winbase.h>
#endif /* _WIN32 */

/// \addtogroup EXCEPT_GROUP
/// \{

#ifndef _WITHOUT_THREAD_EXCEPTIONS

/// Throw exception based on filename and line number.
#define THREAD_EXCEPT_THROW(x) \
    ThreadException THREAD_EXCEPT(PTHREAD_EXCEPTION_PARAMS, x); \
    throw(THREAD_EXCEPT)

/// Catch and re-throw an exception.
#define THREAD_EXCEPT_CATCH_AND_RAISE(x) \
    catch(ThreadException&) { \
        throw; \
    }

/// Begin catch block with no reporter.
#define THREAD_EXCEPT_CATCH_BEGIN_NOREP \
	catch(ThreadException& exc) { \
		exc.what(); // To eliminate warning about unreferenced variable

/// Begin catch block.
#define THREAD_EXCEPT_CATCH_BEGIN(x) \
    catch(ThreadException& exc) { \
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

#else /* _WITHOUT_THREAD_EXCEPTIONS */

#define THREAD_EXCEPT_THROW(x)

#define THREAD_EXCEPT_CATCH_AND_RAISE(x)

#define THREAD_EXCEPT_CATCH_BEGIN(x) \
    {

#define THREAD_EXCEPT_CATCH_ALL(x) \
    } { \

#define THREAD_EXCEPT_CATCH_BEGIN_NOREP \
    {

#define THREAD_EXCEPT_CATCH_END \
    }

#define THREAD_EXCEPT_TRACE_BEGIN \
    {

#define THREAD_EXCEPT_TRACE_END \
    }

#define THREAD_TRY

#endif /* _WITHOUT_THREAD_EXCEPTIONS */
/// \}

///
/// \class ThreadException
/// Thread class library exception.
///

class PTHREADCLASSLIB_API ThreadException : public PThreadBaseException
{
public:
    /// Constructor
    /// \details Constructs exception object with filename and line number.
    /// \param FileName : name of source file.
    /// \param LineNum : number of code line.
    /// \param errcode : error numeric code.
    ThreadException(const char* FileName, int LineNum, int errcode);
    /// Constructor
    /// \details Constructs exception object with filename, line number and text message
    /// \param FileName : name of source file.
    /// \param LineNum : number of code line.
    /// \param Message : user message.
    ThreadException(const char* FileName, int LineNum, const char* Message);
    /// Destructor
    /// \details Intentionally empty
    virtual ~ThreadException()
    {
    }

    /// Reports exception's properties.
    /// \return String buffer with report text.
    virtual const char* what(); // Overload

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
    int getErrNum()
    {
        return m_ErrCode;
    }

    /// Sets error number.
    /// \param Num : error number.
    void setErrNum(int Num = 0)
    {
        m_ErrCode = Num;
    }


protected:
    int m_ErrCode; ///< Error code.
    char lpMsgBuf[256]; ///< Error message buffer.
    char m_Message[256]; ///< UserMessage buffer.
};

#endif

