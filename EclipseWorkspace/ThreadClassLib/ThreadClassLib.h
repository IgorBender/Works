/* ThreadClassLib.h
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

/*! \mainpage C++ Thread Class Library
 *
 * \author Igor Bender.
 *
 * \section intro_sec Introduction
 *
 *
 * <b>C++ Thread Class Library</b> is designed in aim to be a convenient class wrapping on C++ threads.
 * The <b>TCL</b> is based on C++11 standard features.
 * The <b>TCL</b> may be used as a basis for further class libraries providing
 * extended services. The TCL is still under construction and this
 * document may be not fully updated.
 * <br>
 * The <b>TCL</b> provides classes for threads, mutual exclusion locks
 * and condition variables.
 *
 *
 */

#ifndef THREADCLASSLIB_H
#define THREADCLASSLIB_H

#ifdef _WIN32
#ifdef THREADCLASSLIB_EXPORTS
#define THREADCLASSLIB_API __declspec(dllexport)
#else
#define THREADCLASSLIB_API __declspec(dllimport)
#endif
#else // _WIN32
#define THREADCLASSLIB_API
#endif // _WIN32

namespace ThreadClassLib
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
void THREADCLASSLIB_API getVersion(VersionTriple& Versions);
/// \}
} // end of ThreadClassLib namespace

#endif // THREADCLASSLIB_H
