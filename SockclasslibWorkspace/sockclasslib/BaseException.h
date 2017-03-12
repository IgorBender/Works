/* BaseException.h
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

#ifndef _BASEEXCEPTION_H
#define _BASEEXCEPTION_H

#ifdef _WIN32
#pragma warning(disable : 4786)
#pragma warning(disable : 4251)
// #pragma warning(disable : 4275)
#endif

#include <string.h>


#ifndef GCC_VERSION
/// GCC_VERSION
#define GCC_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
#endif /* GCC_VERSION */

#include <vector>
#if GCC_VERSION >= 3001
#include <sstream>
#define repstream stringstream
#else
#include <strstream>
#include <string>
/// repstream
#define repstream strstream
#endif

#if (__QNX__ || __VXWORKS__)
#include <string>
#endif

#include <sockclasslib.h>

/// \file 
///
/// \defgroup EXCEPT_GROUP Exceptions Group
/// 
/// \{

/// Exception parameters includes filename and line number.
#define EXCEPTION_PARAMS __FILE__, __LINE__


///
/// \class BaseException
/// Base class for socket class library exceptions
///

class SOCKLIB_API BaseException
{
public:
    /// Constructor
    /// \details Constructs exception object with filename and line number
    /// \param FileName : name of source file.
    /// \param LineNum : number of code line.
    BaseException(const char* FileName, int LineNum)
    {
        ::strcpy(m_FileName, FileName);
        m_LineNum = LineNum;
    }
    /// Constructor.
    /// \details Copy constructor
    BaseException(const BaseException& exp)
    {
        ::strcpy(m_FileName, exp.m_FileName);
        m_LineNum = exp.m_LineNum;
    }
    /// Destructor
    /// \details Intentionally empty
    virtual ~BaseException()
    {
    }

    /// Reports exception's properties.
    /// \return String buffer with report text.
    virtual const char* what()
    {
        std::repstream report;
        report << "Exception in ";
        report << "file : " << m_FileName << ", line : " << m_LineNum << std::endl;
        report << std::ends;
        repstr = report.str();
        return repstr.c_str();
    }
    
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


#endif /* _BASEEXCEPTION_H */
