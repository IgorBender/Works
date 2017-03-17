/* PThreadBaseException.h
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

#ifndef _PTHREADBASEEXCEPTION_H
#define _PTHREADBASEEXCEPTION_H

#include <PThreadClassLib.h>

#ifdef _WIN32
#pragma warning(disable : 4786)
#pragma warning(disable : 4251)
// #pragma warning(disable : 4275)
#endif

#include <string.h>

#ifndef GCC_VERSION
#define GCC_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
#endif /* GCC_VERSION */

#include <vector>
#if (GCC_VERSION >= 3001 || defined _WIN32)
#include <sstream>
#define pthrrepstream stringstream
#else
#include <strstream>
#include <string>
#define pthrrepstream strstream
#endif

#if (__QNX__ || __VXWORKS__)
#include <string>
#endif

/// \file 
///
/// \defgroup EXCEPT_GROUP Exceptions Group
/// 
/// \{

/// Exception parameters includes filename and line number.
#define PTHREAD_EXCEPTION_PARAMS __FILE__, __LINE__


///
/// \class PThreadBaseException
/// Base class for thread class library exceptions
///
class PTHREADCLASSLIB_API PThreadBaseException {
public:
    /// Constructor
    /// \details Constructs exception object with filename and line number
    /// \param FileName : name of source file.
    /// \param LineNum : number of code line.
    PThreadBaseException(const char* FileName, int LineNum)
    {
        ::strcpy(m_FileName, FileName);
        m_LineNum = LineNum;
    }
    /// Constructor.
    /// \details Copy constructor
    PThreadBaseException(const PThreadBaseException& exp)
    {
        ::strcpy(m_FileName, exp.m_FileName);
        m_LineNum = exp.m_LineNum;
    }
    /// Destructor
    /// \details Intentionally empty
    virtual ~PThreadBaseException()
    {
    }

    /// Reports exception's properties.
    /// \return String buffer with report text.
    virtual const char* what();
    /// Delivers filename of the exception source.
    /// \return Filename.
    const char* getFileName()
    {
        return m_FileName;
    }
    /// Delivers line number of the exception source.
    /// \return Line number.
    int getLineNum()
    {
        return m_LineNum;
    }

protected:
    char m_FileName[256]; ///< Buffer for filename of the exception source.
    int m_LineNum; ///< Line number of the exception source.
    std::string repstr; ///< String for the exception report will be build to.
};

inline const char* PThreadBaseException::what()
{
    std::pthrrepstream report;
    report << "Exception in ";
    report << "file : " << m_FileName << ", line : " << m_LineNum << std::endl;
    report << std::ends;
    repstr = report.str();
    return repstr.c_str();
}

#endif /* _PTHREADBASEEXCEPTION_H */

