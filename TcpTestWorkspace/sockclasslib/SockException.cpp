/* SockException.cpp
 *
 * Copyright 2000 Igor Bender
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or(at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111 - 1307, USA.
 */

#include <SockException.h>


SockException::SockException(const char* FileName, int LineNum,
                             int errcode) : BaseException(FileName, LineNum)
	{
		m_Message[0] = '\0';
		m_ErrCode = errcode;
	}

SockException::SockException(const char* FileName, int LineNum,
                             const char* Message) : BaseException(FileName, LineNum)
	{
		::strcpy(m_Message, Message);
		m_ErrCode = 0;
	}

const char* SockException::what()
{
    std::repstream report;
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
#ifdef _WIN32
#ifndef UNDER_RTSS
	DWORD FormRes = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		m_ErrCode,
		MAKELANGID(LANG_NEUTRAL,
		SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		256,
		NULL);
	if (FormRes == 0)
	{
		FormRes = LoadString((HINSTANCE)hLibMod, m_ErrCode, (LPTSTR)&lpMsgBuf,
			sizeof(lpMsgBuf));
	}
	if (FormRes == 0)
	{
		return m_Message;
	}
#else /* UNDER_RTSS */
	strcpy(lpMsgBuf, "");
#endif /* UNDER_RTSS */
#else /* _WIN32 */
	::strcpy(lpMsgBuf, strerror(m_ErrCode));
	strcat(lpMsgBuf, "\n");
#endif /* _WIN32 */
	
	return reinterpret_cast < char* >(lpMsgBuf);
}

bool SockException::formatMessage(char(*MsgBuf)[256]) 
{
	if (0 == getErrNum())
		return false;
#ifdef _WIN32
#ifndef UNDER_RTSS
	DWORD FormRes = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		getErrNum(),
		MAKELANGID(LANG_NEUTRAL,
		SUBLANG_DEFAULT),
		(LPTSTR) MsgBuf,
		256,
		NULL);
	if (FormRes == 0)
	{
		FormRes = LoadString((HINSTANCE)hLibMod, getErrNum(), (LPTSTR)MsgBuf,
			256);
	}
	if (FormRes == 0)
	{
		return false;
	}
#else /* UNDER_RTSS */
	strcpy(*MsgBuf, "");
#endif /* UNDER_RTSS */
#else /* _WIN32 */
	
	::strcpy(*MsgBuf, strerror(getErrNum()));
	strcat(*MsgBuf, "\n");
#endif /* _WIN32 */
	return true;
}
