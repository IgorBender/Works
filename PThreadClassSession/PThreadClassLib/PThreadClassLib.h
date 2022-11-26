/* PThreadClassLib.h
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

/*! \mainpage POSIX Thread Class Library
 * 
 * \author Igor Bender.
 * 
 * \section intro_sec Introduction
 *
 * 
 * <b>POSIX Thread Class Library</b> is designed in aim to be a cross-platform thread class library. The <b>PTCL</b> is
 * successfully ported on Linux, Solaris, QNX, VxWorks and Windows platforms. The
 * <b>PTCL</b> may be used as a basis for further class libraries providing
 * extended services. The PTCL is still under construction and this
 * document may be not fully updated.
 * <br>
 * The <b>PTCL</b> provides classes for POSIX threads, mutual exclusion locks,
 * read-write locks and condition variables.
 *
 * 
 */

#ifndef PTHREADCLASSLIB_H
#define PTHREADCLASSLIB_H

#ifdef _MSC_VER
#ifdef PTHREADCLASSLIB_EXPORTS
#define PTHREADCLASSLIB_API __declspec(dllexport)
#else
#define PTHREADCLASSLIB_API __declspec(dllimport)
#endif
#else // _MSC_VER
#define PTHREADCLASSLIB_API
#endif // _MSC_VER

#define PTHREAD_INFINITE 0UL

namespace PThreadClassLib
{
/// \file 
///
/// \defgroup LIB_GROUP Library Auxiliaries.
/// 
/// \{
    /// Structure for library versions.
    struct VersionTriple
    {
        int Major; ///< Major version number.
        int Minor; ///< Minor version number.
        int SubMinor; ///< Minor subversion number.
    };

    /// Get library version.
    /// \param Versions : VersionTriple structure to be filled with versions.
    void PTHREADCLASSLIB_API getVersion(VersionTriple& Versions);
/// \}
}


#endif // PTHREADCLASSLIB_H
