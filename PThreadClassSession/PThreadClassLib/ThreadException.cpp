/* ThreadException.cpp
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

#include "ThreadException.h"


ThreadException::ThreadException(const char* FileName, int LineNum,
                                 int errcode) :
#ifdef _TRACE_THREAD_EXCEPTIONS
        PThreadTraceException(errcode, FileName, LineNum)
#else
        PThreadBaseException(FileName, LineNum)
#endif
{
    m_Message[0] = '\0';
    m_ErrCode = errcode;
}

ThreadException::ThreadException(const char* FileName, int LineNum,
                                 const char* Message) :
#ifdef _TRACE_THREAD_EXCEPTIONS
        PThreadTraceException(Message, FileName, LineNum)
#else
        PThreadBaseException(FileName, LineNum)
#endif
{
    ::strcpy(m_Message, Message);
    m_ErrCode = 0;
}

const char* ThreadException::what()
{
    pthrrepstream report;
    if(getErrCode())
    {
        report << strerror(m_ErrCode) << " ";
#ifdef _TRACE_THREAD_EXCEPTIONS
        report << PThreadTraceException::what();
        report << std::ends;
        repstr = report.str();
        return repstr.c_str();
#else
        report << PThreadBaseException::what();
        report << std::ends;
        repstr = report.str();
        return repstr.c_str();
#endif

    }
    else
    {
#ifdef _TRACE_THREAD_EXCEPTIONS
        report << PThreadTraceException::what();
        report << std::ends;
        repstr = report.str();
        return repstr.c_str();
#else
        report << m_Message << std::endl;
        report << PThreadBaseException::what();
        report << std::ends;
        repstr = report.str();
        return repstr.c_str();
#endif
    }
}
