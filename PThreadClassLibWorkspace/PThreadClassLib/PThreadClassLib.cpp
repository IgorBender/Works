/* PThreadClassLib.cpp
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


#ifdef _WIN32
#include <windows.h>
#endif // _WIN32
#include "PThreadClassLib.h"

#ifdef _WIN32
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

//PTHREADCLASSLIB_API int DummyInt = 0; // To create LIB

#endif // _WIN32

/// Get socket class library version.
/// \param Versions : VersionTriple structure to be filled with versions.
void PTHREADCLASSLIB_API PThreadClassLib::getVersion(PThreadClassLib::VersionTriple& Versions)
{
    Versions.Major = 2;
    Versions.Minor = 3;
    Versions.SubMinor = 6;
}
