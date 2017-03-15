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

namespace ThreadClassLib
{
ThreadException::ThreadException(const char* FileName, int LineNum,
                                 int errcode)
{
    ::strcpy(m_FileName, FileName);
    m_LineNum = LineNum;
    m_Message[0] = '\0';
    m_ErrCode = errcode;
}

ThreadException::ThreadException(const char* FileName, int LineNum,
                                 const char* Message)
{
    ::strcpy(m_FileName, FileName);
    m_LineNum = LineNum;
    ::strcpy(m_Message, Message);
    m_ErrCode = 0;
}

const char* ThreadException::what()
{
    thrrepstream report;
    if(getErrCode())
    {
        report << "Exception in ";
        report << "file : " << m_FileName << ", line : " << m_LineNum;
        report << "error : " << strerror(m_ErrCode) << " ";
        report << std::endl << std::ends;
        repstr = report.str();
        return repstr.c_str();
    }
    else
    {
        report << m_Message << std::endl;
        report << "Exception in ";
        report << "file : " << m_FileName << ", line : " << m_LineNum << std::endl;
        repstr = report.str();
        return repstr.c_str();
    }
}
} // end of ThreadClassLib namespace
