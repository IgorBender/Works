/* SockException.cpp
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

#include <SockException.h>


SockException::SockException(const char* FileName, int LineNum,
                             int errcode, SOCKET Sock) : BaseException(FileName, LineNum)
	{
		m_Message[0] = '\0';
		m_ErrCode = errcode;
        m_Sock = Sock;
	}

SockException::SockException(const char* FileName, int LineNum,
                             const char* Message, SOCKET Sock) : BaseException(FileName, LineNum)
	{
		::strcpy(m_Message, Message);
		m_ErrCode = 0;
        m_Sock = Sock;
    }

const char* SockException::what()
{
    repstream report;
    if (getErrCode())
    {
        report << formatMessage();
        report << BaseException::what();
        report << std::ends;
        repstr = report.str();
        return repstr.c_str();
    }
    else
    {
        report << m_Message << std::endl;
        report << BaseException::what();
        report << std::ends;
        repstr = report.str();
        return repstr.c_str();
    }
}

char* SockException::formatMessage()
{
#ifdef _MSC_VER
	DWORD FormRes = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		m_ErrCode,
		MAKELANGID(LANG_NEUTRAL,
		SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		256,
		nullptr);
	if (FormRes == 0)
	{
		FormRes = LoadString((HINSTANCE)hLibMod, m_ErrCode, (LPTSTR)&lpMsgBuf,
			sizeof(lpMsgBuf));
	}
	if (FormRes == 0)
	{
		return m_Message;
	}
#else /* _MSC_VER */
	::strcpy(lpMsgBuf, strerror(m_ErrCode));
	strcat(lpMsgBuf, "\n");
#endif /* _MSC_VER */
	
	return reinterpret_cast < char* >(lpMsgBuf);
}

bool SockException::formatMessage(char(*MsgBuf)[256]) 
{
	if (0 == getErrNum())
		return false;
#ifdef _MSC_VER
	DWORD FormRes = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		getErrNum(),
		MAKELANGID(LANG_NEUTRAL,
		SUBLANG_DEFAULT),
		(LPTSTR) MsgBuf,
		256,
		nullptr);
	if (FormRes == 0)
	{
		FormRes = LoadString((HINSTANCE)hLibMod, getErrNum(), (LPTSTR)MsgBuf,
			256);
	}
	if (FormRes == 0)
	{
		return false;
	}
#else /* _MSC_VER */
	
	::strcpy(*MsgBuf, strerror(getErrNum()));
	strcat(*MsgBuf, "\n");
#endif /* _MSC_VER */
	return true;
}
